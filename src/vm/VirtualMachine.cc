#include "VirtualMachine.hh"
#include "RuntimeException.hh"
#include "src/compiler/ast/BinaryExpression.hh"
#include "src/types/Object.hh"
#include "src/types/ObjectClosure.hh"
#include "src/types/OpCode.hh"
#include "src/types/Value.hh"

namespace sciscript {

void VirtualMachine::execute(CompilerOutput& output) {
  globals.clear();
  globals.resize(output.numGlobals);
  openUpvalues = nullptr;

  // Initialize the first call frame
  callFrames.push_back(CallFrame{
    .chunk = &output.root,
    .localsOffset = 0,
    .returnAddress = 0
  });

  for(int position=0;position<callFrames.back().chunk->bytecode.size();) {
    std::vector<uint8_t>& bytecode = callFrames.back().chunk->bytecode;
    std::vector<Literal>& literals = callFrames.back().chunk->literals;
    uint8_t instruction = bytecode[position];

    collectGarbage(); // TODO: move this from here.

    switch(instruction) {
      case OP_RETURN: {
        if(callFrames.size() == 1) {
          valueStack.clear();
          callFrames.pop_back();
          return;
        }

        Value result = valueStack.back();
        valueStack.pop_back();

        // Get current call frame
        CallFrame& callFrame = callFrames.back();
        closeUpvalues(callFrame.localsOffset);

        while(valueStack.size() > callFrame.localsOffset) {
          valueStack.pop_back();
        }

        valueStack.push_back(result);
        position = callFrame.returnAddress;
        callFrames.pop_back();
        break;
      }
      case OP_ADD: {
        numericOp(bytecode, BinaryOperation::ADDITION);
        position++;
        break;
      }
      case OP_MINUS: {
        numericOp(bytecode, BinaryOperation::SUBTRACTION);
        position++;
        break;
      }
      case OP_MULT: {
        numericOp(bytecode, BinaryOperation::MULTIPLICATION);
        position++;
        break;
      }
      case OP_DIV: {
        numericOp(bytecode, BinaryOperation::DIVISION);
        position++;
        break;
      }
      case OP_MOD: {
        numericOp(bytecode, BinaryOperation::MODULUS);
        position++;
        break;
      }
      case OP_NEG: {
        position++;
        Value v = valueStack.back();
        valueStack.pop_back();

        if(v.type == ValueType::NUMBER) {
          valueStack.push_back(Value::fromDouble(-v.as.number));
        } else if(v.type == ValueType::BOOL) {
          valueStack.push_back(Value::fromBool(-v.as.boolean));
        } else {
          throw new RuntimeException("Unexpected type");
        }

        break;
      }
      case OP_EQUALS: {
        comparisonOp(bytecode, BinaryOperation::EQUALITY);
        position++;
        break;
      }
      case OP_CONSTANT: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        valueStack.push_back(Value::fromLiteral(literals[offset]));
        position += size + 1;
        break;
      }
      case OP_SET_GLOBAL: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        Value value = valueStack.back();
        globals[offset] = value;
        position += size + 1;
        break;
      }
      case OP_READ_GLOBAL: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        valueStack.push_back(globals[offset]);
        position += size + 1;
        break;
      }
      case OP_READ_LOCAL: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        Value value = valueStack[callFrames.back().localsOffset + offset];
        valueStack.push_back(value);
        position += size + 1;
        break;
      }
      case OP_SET_LOCAL: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        Value value = valueStack.back();
        valueStack[callFrames.back().localsOffset + offset] = value;
        position += size + 1;
        break;
      }
      case OP_READ_UPVALUE: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        position += size + 1;
        ObjectUpvalue* upvalue = callFrames.back().closure->upvalues[offset];
        if(upvalue->isClosed) {
          valueStack.push_back(upvalue->closed);
        } else {
          valueStack.push_back(valueStack[upvalue->stackIndex]);
        }
        break;
      }
      case OP_SET_UPVALUE: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        position += size + 1;
        ObjectUpvalue* upvalue = callFrames.back().closure->upvalues[offset];
        if(upvalue->isClosed) {
          upvalue->closed = valueStack.back();
        } else {
          valueStack[upvalue->stackIndex] = valueStack.back();
        }
        break;
      }
      case OP_CLOSURE: {
        position++;
        // Read function
        auto [offset, size] = readDynamicBytes(bytecode, position);
        position += size + 1;

        ObjectClosure* closure = static_cast<ObjectClosure*>(allocateObject(ObjectType::CLOSURE)); // TODO: Garbage collection
        closure->functionIndex = offset;
        closure->name = "temporary function name";

        Chunk& functionChunk = output.functions[offset];
        int numUpvalues = functionChunk.numUpvalues;

        for(int i=0;i<numUpvalues;i++) {
          uint8_t isLocal = bytecode[position];
          position++;
          auto [index, size] = readDynamicBytes(bytecode, position);
          position += size + 1;

          if(isLocal == 1) {
            closure->upvalues.push_back(getUpvalue(index));
          } else {
            closure->upvalues.push_back(callFrames.back().closure->upvalues[index]);
          }
        }
        valueStack.push_back(Value::fromObject(closure));
        break;
      }
      case OP_POP: {
        position++;
        valueStack.pop_back();
        break;
      }
      case OP_CLOSE_UPVALUE: {
        position++;
        closeUpvalues(valueStack.size() - 1);
        valueStack.pop_back();
        break;
      }
      case OP_PRINT: {
        position++;
        Value v = valueStack.back();
        valueStack.pop_back();
        printValue(v);
        break;
      }
      case OP_CALL: {
        // TODO: Add a stack overflow exception
        position++;
        auto [numArgs, size] = readDynamicBytes(bytecode, position);
        position += size + 1;

        // Read arguments
        std::vector<Value> args;
        for(int i=0;i<numArgs;i++) {
          args.push_back(valueStack.back());
          valueStack.pop_back();
        }

        // Read closure
        Value v = valueStack.back();
        valueStack.pop_back();

        // Put arguments back on stack
        for(int i=numArgs-1;i>=0;i--) {
          valueStack.push_back(args[i]);
        }

        if(v.type != ValueType::OBJECT || v.as.object->type != ObjectType::CLOSURE) {
          throw new RuntimeException("You can only call functions");
        }
        ObjectClosure* closure = static_cast<ObjectClosure*>(v.as.object);
        callFrames.push_back(CallFrame{
          .localsOffset = static_cast<int>(valueStack.size() - numArgs),
          .returnAddress = position,
          .chunk = &output.functions[closure->functionIndex],
          .closure = closure,
        });
        position = 0;
        break;
      }
      case OP_JUMP_FALSE: {
        position++;
        auto [offset, size] = readDynamicBytes(bytecode, position);
        Value comparison = valueStack.back();
        valueStack.pop_back();

        if(comparison.type != ValueType::BOOL) {
          throw new RuntimeException("Expected boolean value in jump");
        }

        if(!comparison.as.boolean) {
          position += offset;
        }
        position += size + 1;
        break;
      }
      default: {
        position++;
      }
    } 
  }
  
  for(Object* object : objects) {
    delete object;
  }
  objects.clear();
  output.free();
}

std::pair<int, int> VirtualMachine::readDynamicBytes(std::vector<uint8_t>& bytecode, int position) {
  uint8_t size = bytecode[position];
  int result = 0;
  for(int i=0;i<size;i++) {
    result = result | (bytecode[position + 1 + i] << i * 8);
  }
  return std::make_pair(result, size);
}

void VirtualMachine::numericOp(std::vector<uint8_t>& bytecode, BinaryOperation op) {
  Value v1 = valueStack.back();
  valueStack.pop_back();
  Value v2 = valueStack.back();
  valueStack.pop_back();

  if(v1.type != ValueType::NUMBER || v2.type != ValueType::NUMBER) {
    throw new RuntimeException("Unexpected types in binary op.");
  }

  double v1n = v1.as.number;
  double v2n = v2.as.number;

  switch(op) {
    case BinaryOperation::ADDITION:
      valueStack.push_back(Value::fromDouble(v2n + v1n));
      break;
    case BinaryOperation::SUBTRACTION:
      valueStack.push_back(Value::fromDouble(v2n - v1n));
      break;
    case BinaryOperation::MULTIPLICATION:
      valueStack.push_back(Value::fromDouble(v2n * v1n));
      break;
    case BinaryOperation::DIVISION:
      valueStack.push_back(Value::fromDouble(v2n / v1n));
      break;
    case BinaryOperation::MODULUS:
      valueStack.push_back(Value::fromDouble(std::fmod(v2n, v1n)));
      break;
    default:
      break;
  }
}

void VirtualMachine::comparisonOp(std::vector<uint8_t>& bytecode, BinaryOperation op) {
  Value v1 = valueStack.back();
  valueStack.pop_back();
  Value v2 = valueStack.back();
  valueStack.pop_back();

  switch(op) {
    case BinaryOperation::EQUALITY: {
      bool hasNil = v1.type == ValueType::NIL || v2.type == ValueType::NIL;

      if(v1.type != v2.type && !hasNil) {
        throw new RuntimeException("Cannot compare different types");
      }

      if(hasNil && v1.type == ValueType::NIL && v2.type == ValueType::NIL) {
        valueStack.push_back(Value::fromBool(true));
        return;
      } else {
        if(hasNil) {
          valueStack.push_back(Value::fromBool(false));
          return;
        }
      }

      // Compare
      switch(v1.type) {
        case ValueType::NUMBER:
          valueStack.push_back(Value::fromBool(v1.as.number == v2.as.number));
          break;
        case ValueType::BOOL:
          valueStack.push_back(Value::fromBool(v1.as.boolean == v2.as.boolean));
          break;
        case ValueType::NIL:
          valueStack.push_back(Value::fromBool(true));
          break;
        case ValueType::OBJECT:
          valueStack.push_back(Value::fromBool(v1.as.object == v2.as.object));
          break;
      }
      break;
    }
    default:
      break;
  }
}

void VirtualMachine::printValue(Value v) {
  switch(v.type) {
    case ValueType::NUMBER:
      std::cout << v.as.number << std::endl;
      break;
    case ValueType::BOOL:
      std::cout << (v.as.boolean ? "true" : "false") << std::endl;
      break;
    case ValueType::NIL:
      std::cout << "nil" << std::endl;
      break;
    case ValueType::OBJECT:
      std::cout << "object" << std::endl;
      break;
    default:
      throw new RuntimeException("Unexpected types in print");
  }
}

ObjectUpvalue* VirtualMachine::getUpvalue(int stackIndex) {
  ObjectUpvalue* prev = nullptr;
  ObjectUpvalue* upvalue = openUpvalues;

  while(upvalue != nullptr && upvalue->stackIndex > stackIndex) {
    prev = upvalue;
    upvalue = upvalue->next;
  }

  if(upvalue != nullptr && upvalue->stackIndex == stackIndex) { 
    return upvalue;
  }

  ObjectUpvalue* createdUpvalue = static_cast<ObjectUpvalue*>(allocateObject(ObjectType::UPVALUE));
  createdUpvalue->next = upvalue;
  createdUpvalue->stackIndex = stackIndex;
  createdUpvalue->isClosed = false;

  if(prev == nullptr) {
    openUpvalues = createdUpvalue;
  } else {
    prev->next = createdUpvalue;
  }

  return createdUpvalue;
}

void VirtualMachine::closeUpvalues(int lastIndex) {
  while(openUpvalues != nullptr && openUpvalues->stackIndex >= lastIndex) {
    ObjectUpvalue* upvalue = openUpvalues;
    upvalue->closed = valueStack[upvalue->stackIndex];
    upvalue->isClosed = true;
    openUpvalues = upvalue->next;
  }
}

Object* VirtualMachine::allocateObject(ObjectType type) {
  switch(type) {
    case ObjectType::CLOSURE: {
      Object* obj = new ObjectClosure();
      objects.push_back(obj);
      return obj;
    }
    case ObjectType::UPVALUE: {
      Object* obj = new ObjectUpvalue();
      objects.push_back(obj);
      return obj;
    }
  }
}

void VirtualMachine::collectGarbage() {
  if(debugGarbageCollector) {
    std::cout << "-- Initialize garbage collection --" << std::endl;
  }
  markRoots();
  traceReferences();
  sweep();
}

void VirtualMachine::markRoots() {
  // (1) Mark all values on stack
  for(Value value : valueStack) {
    markValue(value);
  }

  // (2) Mark all globals
  for(Value value : globals) {
    markValue(value);
  }

  // (3) Mark all call frame closure objects
  for(CallFrame& frame : callFrames) {
    markObject(static_cast<Object*>(frame.closure));
  }

  // (4) Mark all open upvalues
  for(ObjectUpvalue* upvalue = openUpvalues;upvalue != nullptr;upvalue=upvalue->next) {
    markObject(static_cast<Object*>(upvalue));
  }
}

void VirtualMachine::traceReferences() {
  while(!gcWorkingStack.empty()) {
    Object* obj = gcWorkingStack.back();
    gcWorkingStack.pop_back();
    traverseObjectReferences(obj);
  }
}

void VirtualMachine::sweep() {
  for(int i=objects.size()-1;i>=0;i--) {
    Object* object = objects[i];

    if(object->isMarked) {
      object->isMarked = false;
    } else {
      if(debugGarbageCollector) {
        std::cout << "Deleted object with value ";
        printValue(Value::fromObject(object));
      }
      Object* unreached = object;
      objects.erase(objects.begin() + i);
      delete unreached;
    }
  }
}

void VirtualMachine::traverseObjectReferences(Object* object) {
  if(debugGarbageCollector) {
    std::cout << "traverse object references of " << object << " with value ";
    printValue(Value::fromObject(object));
  }

  switch(object->type) {
    case ObjectType::CLOSURE: {
      ObjectClosure* closure = static_cast<ObjectClosure*>(object);
      for(ObjectUpvalue* upvalue : closure->upvalues) {
        markObject(static_cast<Object*>(upvalue));
      }
      break;
    }
    case ObjectType::UPVALUE: {
      ObjectUpvalue* upvalue = static_cast<ObjectUpvalue*>(object);
      markValue(upvalue->closed);
    }
  }
}

void VirtualMachine::markValue(Value value) {
  if(value.type == ValueType::OBJECT) {
    markObject(value.as.object);
  }
}

void VirtualMachine::markObject(Object* object) {
  if(object != nullptr) {
    if(object->isMarked) return;
    if(debugGarbageCollector) {
      std::cout << "mark object " << object << " with value ";
      printValue(Value::fromObject(object));
    }
    object->isMarked = true;
    gcWorkingStack.push_back(object);
  }
}

}