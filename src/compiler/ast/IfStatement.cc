#include "IfStatement.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void IfStatement::emitBytecode(Chunk& chunk) {
  int OP_JUMP_SIZE = 3;

  std::vector<int> bodyOffsets;
  std::vector<int> jumpOffsets;

  // (1) Emit true statement
  condition->emitBytecode(chunk);
  chunk.emitByte(OP_JUMP_FALSE);
  bodyOffsets.push_back(chunk.bytecode.size());
  trueStatement->emitBytecode(chunk);
  jumpOffsets.push_back(chunk.bytecode.size());
  chunk.emitByte(OP_JUMP);
  chunk.emitByte(0);
  chunk.emitByte(0);

  int jump = jumpOffsets[0] - bodyOffsets[0] + OP_JUMP_SIZE;
  uint8_t jumpLow = ((jump >> 0) & 0xFF);
  uint8_t jumpHigh = ((jump >> 8) & 0XFF);
  chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[0] + 0, jumpLow);
  chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[0] + 1, jumpHigh);

  // (2) Emit else if statements
  int elifIndex = 1;
  for(auto elif : elifStatements) {
    elif.first->emitBytecode(chunk);
    chunk.emitByte(OP_JUMP_FALSE);
    bodyOffsets.push_back(chunk.bytecode.size());
    elif.second->emitBytecode(chunk);
    jumpOffsets.push_back(chunk.bytecode.size());
    chunk.emitByte(OP_JUMP);
    chunk.emitByte(0);
    chunk.emitByte(0);

    jump = jumpOffsets[elifIndex] - bodyOffsets[elifIndex] + OP_JUMP_SIZE;
    jumpLow = ((jump >> 0) & 0xFF);
    jumpHigh = ((jump >> 8) & 0XFF);
    chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[elifIndex] + 0, jumpLow);
    chunk.bytecode.insert(chunk.bytecode.begin() + bodyOffsets[elifIndex] + 1, jumpHigh);
    elifIndex++;
  }

  // (3) Emit else statement
  int elseStart = chunk.bytecode.size();
  if(elseStatement != nullptr) {
    elseStatement->emitBytecode(chunk);
  }
  int elseLength = chunk.bytecode.size() - elseStart;

  // (4) Set OP_JUMP skip lengths
  for(int i=0;i<bodyOffsets.size();i++) {
    int endPoint = elseStart + elseLength;
    int startPoint = jumpOffsets[i] + OP_JUMP_SIZE;
    jump = endPoint - startPoint;
    jumpLow = ((jump >> 0) & 0xFF);
    jumpHigh = ((jump >> 8) & 0XFF);
    std::cout << jumpOffsets[i] << std::endl;
    chunk.bytecode[jumpOffsets[i] + OP_JUMP_SIZE] = jumpLow;
    chunk.bytecode[jumpOffsets[i] + OP_JUMP_SIZE + 1] = jumpHigh;
  }
}

}