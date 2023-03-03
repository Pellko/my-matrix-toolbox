#include "ControlFlowBuilder.hh"
#include "src/types/OpCode.hh"
#include <iostream>

namespace mymatrixtoolbox {

ControlFlowBuilder ControlFlowBuilder::begin() {
  ControlFlowBuilder builder;
  return builder;
}

ControlFlowBuilder& ControlFlowBuilder::addClause(std::function<void (Chunk&)> condition, std::function<void (Chunk& )> body) {
  clauses.push_back({condition, body});
  return *this;
}

ControlFlowBuilder& ControlFlowBuilder::addElseClause(std::function<void (Chunk&)> body) {
  hasElse = true;
  elseBody = body;
  return *this;
}

void ControlFlowBuilder::emitBytecode(Chunk& chunk) {
  std::vector<int> bodyOffsets;
  std::vector<int> jumpOffsets;
  int OP_JUMP_SIZE = 3;
  int index = 0;

  // Emit clauses
  for(auto& clause : clauses) {
    clause.first(chunk);
    chunk.emitByte(OP_JUMP_FALSE);
    bodyOffsets.push_back(chunk.bytecode.size());
    clause.second(chunk);
    jumpOffsets.push_back(chunk.bytecode.size());
    chunk.emitByte(OP_JUMP);
    chunk.emitByte(0);
    chunk.emitByte(0);
    int jump = jumpOffsets[index] - bodyOffsets[index] + OP_JUMP_SIZE;
    uint8_t jumpLow = ((jump >> 0) & 0xFF);
    uint8_t jumpHigh = ((jump >> 8) & 0XFF);
    chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[index] + 0, jumpLow);
    chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[index] + 1, jumpHigh);
    index++;
  }

  // Emit else
  int elseStart = chunk.bytecode.size();
  if(hasElse) {
    elseBody(chunk);
  }
  int elseLength = chunk.bytecode.size() - elseStart;

  // Set OP_JUMP skip lengths
  for(int i=0;i<bodyOffsets.size();i++) {
    int endPoint = elseStart + elseLength;
    int startPoint = jumpOffsets[i] + OP_JUMP_SIZE + 2;
    int jump = endPoint - startPoint;
    uint8_t jumpLow = ((jump >> 0) & 0xFF);
    uint8_t jumpHigh = ((jump >> 8) & 0XFF);
    chunk.bytecode[jumpOffsets[i] + OP_JUMP_SIZE] = jumpLow;
    chunk.bytecode[jumpOffsets[i] + OP_JUMP_SIZE + 1] = jumpHigh;
  }
}

}