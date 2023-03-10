#include "Disassembler.hh"
#include "src/types/Literal.hh"
#include "src/types/OpCode.hh"
#include <iostream>

namespace mymatrixtoolbox {

void Disassembler::disassemble(CompilerOutput& output) {
  std::cout << "Chunk <0>" << std::endl;
  disassembleChunk(output.root, output);
  std::cout << std::endl;

  for(int i=0;i<output.functions.size();i++) {
    Chunk& chunk = output.functions[i];
    std::cout << "Chunk <" << i+1 << ">:" << std::endl;
    disassembleChunk(chunk, output);
    std::cout << std::endl;
  }

  // TODO: Disassemble modules properly
  
  // for(int i=0;i<output.modules.size();i++) {
  //   std::cout << "Module <" << i << ">" << std::endl;
  //   disassembleChunk(output.modules[i].root, output);
  //   std::cout << std::endl;

  //   for(int j=0;j<output.modules[i].functions.size();j++) {
  //     Chunk& chunk = output.modules[i].functions[j];
  //     std::cout << "Chunk <" << j+1 << ">:" << std::endl;
  //     disassembleChunk(chunk, output);
  //     std::cout << std::endl;
  //   } 
  // }
}

void Disassembler::disassembleChunk(Chunk& chunk, CompilerOutput& output) {
  for(int offset=0;offset<chunk.bytecode.size();) {
    uint8_t instruction = chunk.bytecode[offset];

    std::cout << "<" << offset << "> ";

    switch(instruction) {
      case OP_RETURN:
        offset = simpleInstruction("OP_RETURN", offset);
        break;
      case OP_CONSTANT:
        offset = constantInstruction("OP_CONSTANT", offset, chunk);
        break;
      case OP_ADD:
        offset = simpleInstruction("OP_ADD", offset);
        break;
      case OP_MINUS:
        offset = simpleInstruction("OP_MINUS", offset);
        break;
      case OP_MULT:
        offset = simpleInstruction("OP_MULT", offset);
        break;
      case OP_DIV:
        offset = simpleInstruction("OP_DIV", offset);
        break;
      case OP_MOD:
        offset = simpleInstruction("OP_MOD", offset);
        break;
      case OP_NEG:
        offset = simpleInstruction("OP_NEG", offset);
        break;
      case OP_PRINT:
        offset = simpleInstruction("OP_PRINT", offset);
        break;
      case OP_POP:
        offset = simpleInstruction("OP_POP", offset);
        break;
      case OP_CLOSE_UPVALUE:
        offset = simpleInstruction("OP_CLOSE_UPVALUE", offset);
        break;
      case OP_EQUALS:
        offset = simpleInstruction("OP_EQUALS", offset);
        break;
      case OP_LT:
        offset = simpleInstruction("OP_LT", offset);
        break;
      case OP_LEQ:
        offset = simpleInstruction("OP_LEQ", offset);
        break;
      case OP_LOGICAL_OR:
        offset = simpleInstruction("OP_LOGICAL_OR", offset);
        break;
      case OP_LOGICAL_AND:
        offset = simpleInstruction("OP_LOGICAL_AND", offset);
        break;
      case OP_READ_GLOBAL:
        offset = readVariable("OP_READ_GLOBAL", offset, chunk);
        break;
      case OP_READ_LOCAL:
        offset = readVariable("OP_READ_LOCAL", offset, chunk);
        break;
      case OP_SET_GLOBAL:
        offset = setVariable("OP_SET_GLOBAL", offset, chunk);
        break;
      case OP_SET_LOCAL:
        offset = setVariable("OP_SET_LOCAL", offset, chunk);
        break;
      case OP_CALL:
        offset = callInstruction("OP_CALL", offset, chunk);
        break;
      case OP_CLOSURE:
        offset = closureInstruction("OP_CLOSURE", offset, chunk, output);
        break;
      case OP_READ_UPVALUE:
        offset = readVariable("OP_READ_UPVALUE", offset, chunk);
        break;
      case OP_SET_UPVALUE:
        offset = setVariable("OP_SET_UPVALUE", offset, chunk);
        break;
      case OP_JUMP_FALSE:
        offset = jumpInstruction("OP_JUMP_FALSE", offset, chunk);
        break;
      case OP_JUMP:
        offset = jumpInstruction("OP_JUMP", offset, chunk);
        break;
      case OP_LOOP:
        offset = jumpInstruction("OP_LOOP", offset, chunk);
        break;
      case OP_MATRIX:
        offset = matrixInstruction("OP_MATRIX", offset, chunk);
        break;
      case OP_READ_MATRIX:
        offset = simpleInstruction("OP_READ_MATRIX", offset);
        break;
      case OP_SET_MATRIX:
        offset = simpleInstruction("OP_SET_MATRIX", offset);
        break;
      case OP_CLASS:
        offset = simpleInstruction("OP_CLASS", offset);
        break;
      case OP_METHOD:
        offset = readVariable("OP_METHOD", offset, chunk);
        break;
      case OP_SET_PROPERTY:
        offset = setVariable("OP_SET_PROPERTY", offset, chunk);
        break;
      case OP_READ_PROPERTY:
        offset = readVariable("OP_READ_PROPERTY", offset, chunk);
        break;
      case OP_MAP:
        offset = mapInstruction("OP_MAP", offset, chunk);
        break;
      case OP_MODULE:
        offset = setVariable("OP_MODULE", offset, chunk);
        break;
      default:
        std::cout << "Unknown opcode " << instruction << "\n";
        offset += 1;
    }
  }
}

int Disassembler::simpleInstruction(std::string instruction, int offset) {
  std::cout << instruction << std::endl;
  return offset + 1;
}

int Disassembler::constantInstruction(std::string instruction, int offset, Chunk& chunk) {
  auto [index, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Index=" << index << ", Value=" << printLiteral(chunk.literals[index]) << std::endl;
  return offset + 2 + size;
}

int Disassembler::readVariable(std::string instruction, int offset, Chunk& chunk) {
  auto [index, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Index=" << index << std::endl;
  return offset + 2 + size;
}

int Disassembler::setVariable(std::string instruction, int offset, Chunk& chunk) {
  auto [index, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Index=" << index << std::endl;
  return offset + 2 + size;
}

int Disassembler::callInstruction(std::string instruction, int offset, Chunk& chunk) {
  auto [index, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Argcount=" << index << std::endl;
  return offset + 2 + size;
}

int Disassembler::closureInstruction(std::string instruction, int offset, Chunk& chunk, CompilerOutput& output) {
  int skip = 1;
  auto [functionIndex, size] = readDynamicBytes(offset + skip, chunk);
  skip += size + 1;
  Chunk& functionChunk = output.functions[functionIndex];
  for(int i=0;i<functionChunk.numUpvalues;i++) {
    skip++;
    auto [index, size] = readDynamicBytes(offset + skip, chunk);
    skip += size + 1;
  }
  std::cout << instruction << std::endl;
  return offset + skip;
}

int Disassembler::jumpInstruction(std::string instruction, int offset, Chunk& chunk) {
  uint8_t lowByte = chunk.bytecode[offset + 1];
  uint8_t highByte = chunk.bytecode[offset + 2];
  int value = lowByte | (highByte << 8);
  std::cout << instruction << " : Skip=" << value << std::endl;
  return offset + 3;
}

int Disassembler::matrixInstruction(std::string instruction, int offset, Chunk& chunk) {
  auto [width, size] = readDynamicBytes(offset + 1, chunk);
  auto [height, size2] = readDynamicBytes(offset + 2 + size, chunk);
  std::cout << instruction << " : Width=" << width << ", Height=" << height << std::endl;
  return offset + 3 + size + size2;
}

int Disassembler::mapInstruction(std::string instruction, int offset, Chunk& chunk) {
  auto [num, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Num of Properties=" << num << std::endl;
  return offset + 2 + size;
}

std::string Disassembler::printLiteral(Literal literal) {
  switch(literal.type) {
    case LiteralType::NUMBER:
      return std::to_string(literal.as.number);
    case LiteralType::STRING:
      return "\"" + *literal.as.str + "\"";
    default:
      return "Unsupported LiteralType";
  }
}

std::pair<int, int> Disassembler::readDynamicBytes(int offset, Chunk& chunk) {
  uint8_t size = chunk.bytecode[offset];
  int index = 0;
  for(int i=0;i<size;i++) {
    index = index | (chunk.bytecode[offset + 1 + i] << i * 8);
  }
  return std::make_pair(index, size);
}

}