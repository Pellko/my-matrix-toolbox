#include "VirtualMachine.hh"

namespace sciscript {

void VirtualMachine::execute(CompilerOutput& output) {
  for(int position = 0; position<output.bytecode.size();) {
    uint8_t instruction = output.bytecode[position];

    switch (instruction) {
      case OP_RETURN:
        break;
      case OP_ADD: {
        position++;
        Value v1 = valueStack.top();
        valueStack.pop();
        Value v2 = valueStack.top();
        valueStack.pop();

        if(IS_NUMBER(v1) && IS_NUMBER(v2)) {
          valueStack.push(NUMBER_VAL(AS_NUMBER(v1) + AS_NUMBER(v2)));
        } else {
          throw new RuntimeException("Unexpected types in add");
        }
        break;
      }
      case OP_MINUS:
        break;
      case OP_MULT:
        break;
      case OP_DIV:
        break;
      case OP_MOD:
        break;
      case OP_NEG:
        break;
      case OP_CONSTANT: {
        position++;
        auto [offset, size] = readDynamicBytes(output.bytecode, position);
        valueStack.push(output.constants[offset]);
        position += 1 + size;
        break;
      }
      case OP_DEFINE_GLOBAL:
        break;
      case OP_PRINT: {
        position++;
        Value v = valueStack.top();
        if(IS_NUMBER(v)) {
          std::cout << AS_NUMBER(v) << std::endl;
          valueStack.pop();
        } else {
          throw new RuntimeException("Unexpected types in print");
        }
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

}