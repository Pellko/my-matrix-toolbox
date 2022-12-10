#include "Disassembler.hh"

namespace sciscript {

void Disassembler::disassemble(CompilerOutput &code, std::string &output) {
  output.clear();
  std::stringstream ss;

  for(int offset=0;offset<code.bytecode.size();) {
    uint8_t instruction = code.bytecode[offset];
    switch(instruction) {
      case OP_RETURN:
        offset = simpleInstruction("OP_RETURN", offset, ss);
        break;
      case OP_CONSTANT:
        offset = constantInstruction("OP_CONSTANT", offset, code, ss);
        break;
      case OP_ADD:
        offset = simpleInstruction("OP_ADD", offset, ss);
        break;
      case OP_MINUS:
        offset = simpleInstruction("OP_MINUS", offset, ss);
        break;
      case OP_MULT:
        offset = simpleInstruction("OP_MULT", offset, ss);
        break;
      case OP_DIV:
        offset = simpleInstruction("OP_DIV", offset, ss);
        break;
      case OP_MOD:
        offset = simpleInstruction("OP_MOD", offset, ss);
        break;
      case OP_NEG:
        offset = simpleInstruction("OP_NEG", offset, ss);
        break;
      case OP_SET_GLOBAL:
        offset = globalInstruction("OP_SET_GLOBAL", offset, code, ss);
        break;
      case OP_READ_GLOBAL:
        offset = globalInstruction("OP_READ_GLOBAL", offset, code, ss);
        break;
      case OP_SET_LOCAL:
        offset = localInstruction("OP_SET_LOCAL", offset, code, ss);
        break;
      case OP_READ_LOCAL:
        offset = localInstruction("OP_READ_LOCAL", offset, code, ss);
        break;
      case OP_PRINT:
        offset = simpleInstruction("OP_PRINT", offset, ss);
        break;
      case OP_POP:
        offset = simpleInstruction("OP_POP", offset, ss);
        break;
      default:
        ss << "Unkown opcode " << instruction << "\n";
        offset += 1;
    }
  }

  ss >> output;
}

int Disassembler::simpleInstruction(std::string instruction, int offset, std::stringstream& ss) {
  ss << instruction << "\n";
  std::cout << instruction << std::endl;
  return offset + 1;
}

int Disassembler::constantInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss) {
  uint8_t constantSize = code.bytecode[offset + 1];
  int index = 0;
  for(int i=0;i<constantSize;i++) {
    index = index | (code.bytecode[offset + 2 + i] << i * 8);
  }
  ss << instruction << " : Index=" << index << "\n";
  std::cout << instruction << " : Index=" << index << ", Value=" << printValue(code.constants[index]) << std::endl;
  return offset + 2 + static_cast<int>(constantSize);
}

int Disassembler::globalInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss) {
  uint8_t constantSize = code.bytecode[offset + 1];
  int index = 0;
  for(int i=0;i<constantSize;i++) {
    index = index | (code.bytecode[offset + 2 + i] << i * 8);
  }
  ss << instruction << " : Index=" << index << "\n";
  std::cout << instruction << " : Index=" << index << ", Name=" << code.globals[index] << std::endl;
  return offset + 2 + static_cast<int>(constantSize);
}

int Disassembler::localInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss) {
  uint8_t constantSize = code.bytecode[offset + 1];
  int index = 0;
  for(int i=0;i<constantSize;i++) {
    index = index | (code.bytecode[offset + 2 + i] << i * 8);
  }
  ss << instruction << " : Index=" << index << "\n";
  std::cout << instruction << " : Index=" << index << std::endl;
  return offset + 2 + static_cast<int>(constantSize);
}

std::string Disassembler::printValue(Value value) {
  switch(value.type) {
    case VAL_BOOL:
      return AS_BOOL(value) ? "true" : "false";
    case VAL_NIL:
      return "nil";
    case VAL_NUMBER:
      return std::to_string(AS_NUMBER(value));
    case VAL_OBJECT:
      return printObject(AS_OBJECT(value));
  }
}

std::string Disassembler::printObject(Object* object) {
  switch(object->type) {
    case ObjectType::STRING:
      return static_cast<ObjectString*>(object)->value;
  }
}

}