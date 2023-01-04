#include "WhileStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void WhileStatement::emitBytecode(Chunk& chunk) {
  int OP_JUMP_SIZE = 3;

  int conditionStart = chunk.bytecode.size();
  condition->emitBytecode(chunk);
  int conditionLength = chunk.bytecode.size() - conditionStart;

  chunk.emitByte(OP_JUMP_FALSE);

  int bodyStart = chunk.bytecode.size();
  body->emitBytecode(chunk);
  int bodyLength = chunk.bytecode.size() - bodyStart;

  chunk.emitByte(OP_LOOP);
  int jump = bodyLength + conditionLength + OP_JUMP_SIZE;
  uint8_t jumpLow = ((jump >> 0) & 0xFF);
  uint8_t jumpHigh = ((jump >> 8) & 0XFF);
  chunk.emitByte(jumpLow);
  chunk.emitByte(jumpHigh);

  jump = bodyLength + OP_JUMP_SIZE;
  jumpLow = ((jump >> 0) & 0xFF);
  jumpHigh = ((jump >> 8) & 0XFF);
  chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 1, jumpLow);
  chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 2, jumpHigh);
}

}