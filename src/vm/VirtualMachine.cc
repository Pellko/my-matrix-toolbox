#include "VirtualMachine.hh"
#include "src/types/ArithmeticType.hh"
#include "src/types/OpCode.hh"
#include "src/vm/RuntimeException.hh"

namespace sciscript {

void VirtualMachine::execute(CompilerOutput& output) {
  globals.clear();
  globals.resize(output.globals.size());

  for(int position = 0; position<output.bytecode.size();) {
    uint8_t instruction = output.bytecode[position];

    switch (instruction) {
      case OP_RETURN: {
        break;
      }
      case OP_ADD: {
        binaryOp(output.bytecode, ArithmeticType::PLUS);
        position++;
        break;
      }
      case OP_MINUS: {
        binaryOp(output.bytecode, ArithmeticType::MINUS);
        position++;
        break;
      }
      case OP_MULT: {
        binaryOp(output.bytecode, ArithmeticType::MULT);
        position++;
        break;
      }
      case OP_DIV: {
        binaryOp(output.bytecode, ArithmeticType::DIV);
        position++;
        break;
      }
      case OP_MOD: {
        binaryOp(output.bytecode, ArithmeticType::MOD);
        position++;
        break;
      }
      case OP_NEG: {
        position++;
        Value v = valueStack.back();
        valueStack.pop_back();

        if(v.type != VAL_NUMBER) {
          throw new RuntimeException("Expected number");
        }
        valueStack.push_back(NUMBER_VAL(-AS_NUMBER(v)));
        break;
      }
      case OP_CONSTANT: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        valueStack.push_back(output.constants[offset]);
        position += size + 1;
        break;
      }
      case OP_SET_GLOBAL: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        Value value = valueStack.back();
        globals[offset] = value;
        position += size + 1;
        break;
      }
      case OP_READ_GLOBAL: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        valueStack.push_back(globals[offset]);
        position += size + 1;
        break;
      }
      case OP_SET_LOCAL: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        Value value = valueStack.back();
        valueStack[offset] = value;
        position += size + 1;
        break;
      }
      case OP_READ_LOCAL: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        Value value = valueStack[offset];
        valueStack.push_back(value);
        position += size + 1;
        break;
      }
      case OP_PRINT: {
        position++;
        Value v = valueStack.back();
        if(IS_NUMBER(v)) {
          std::cout << AS_NUMBER(v) << std::endl;
          valueStack.pop_back();
        } else {
          throw new RuntimeException("Unexpected types in print");
        }
        break;
      }
      case OP_POP: {
        position++;
        valueStack.pop_back();
        break;
      }
      default: {
        position++;
      }
    }
  }
}

std::pair<int, int> VirtualMachine::readDynamicBytes(std::vector<uint8_t>& bytecode, int position) {
  uint8_t size = bytecode[position];
  int result = 0;
  for(int i=0;i<size;i++) {
    result = result | (bytecode[position + 1 + i] << i * 8);
  }
  return std::make_pair(result, size);
}

void VirtualMachine::binaryOp(std::vector<uint8_t>& bytecode, ArithmeticType type) {
  Value v1 = valueStack.back();
  valueStack.pop_back();
  Value v2 = valueStack.back();
  valueStack.pop_back();

  if(!IS_NUMBER(v1) || !IS_NUMBER(v2)) {
    throw new RuntimeException("Unexpected types in binary op.");
  }

  double v1n = AS_NUMBER(v1);
  double v2n = AS_NUMBER(v2);

  switch(type) {
    case ArithmeticType::PLUS:
      valueStack.push_back(NUMBER_VAL(v2n + v1n));
      break;
    case ArithmeticType::MINUS:
      valueStack.push_back(NUMBER_VAL(v2n - v1n));
      break;
    case ArithmeticType::MULT:
      valueStack.push_back(NUMBER_VAL(v2n * v1n));
      break;
    case ArithmeticType::DIV:
      valueStack.push_back(NUMBER_VAL(v2n / v1n));
      break;
    case ArithmeticType::MOD:
      valueStack.push_back(NUMBER_VAL(std::fmod(v1n, v2n)));
      break;
  }
} 

}