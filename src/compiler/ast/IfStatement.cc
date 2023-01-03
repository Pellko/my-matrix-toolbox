#include "IfStatement.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void IfStatement::emitBytecode(Chunk& chunk) {
  condition->emitBytecode(chunk);
  chunk.emitByte(OP_JUMP_FALSE);

  int trueStatementStart = chunk.bytecode.size();
  trueStatement->emitBytecode(chunk);
  int trueStatementLength = chunk.bytecode.size() - trueStatementStart;

  int jump = trueStatementLength;
  uint8_t jumpLow = ((jump >> 0) & 0xFF);
  uint8_t jumpHigh = ((jump >> 8) & 0XFF);
  chunk.bytecode.insert(chunk.bytecode.begin() + trueStatementStart + 0, jumpLow);
  chunk.bytecode.insert(chunk.bytecode.begin() + trueStatementStart + 1, jumpHigh);
}

}