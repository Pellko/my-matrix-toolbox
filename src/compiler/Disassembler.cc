#include "Disassembler.hh"
#include "src/types/Literal.hh"
#include "src/types/OpCode.hh"
#include <iostream>

namespace sciscript {

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
}

void Disassembler::disassembleChunk(Chunk& chunk, CompilerOutput& output) {
  for(int offset=0;offset<chunk.bytecode.size();) {
    uint8_t instruction = chunk.bytecode[offset];
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
  auto [index, size] = readDynamicBytes(offset + 1, chunk);
  std::cout << instruction << " : Skip=" << index << std::endl;
  return offset + 2 + size;
}

std::string Disassembler::printLiteral(Literal literal) {
  switch(literal.type) {
    case LiteralType::NUMBER:
      return std::to_string(literal.as.number);
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