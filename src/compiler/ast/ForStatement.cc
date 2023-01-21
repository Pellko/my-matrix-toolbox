#include "ForStatement.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

// TODO: throw error if the jump is too big.
void ForStatement::emitBytecode(Chunk& chunk) {
  if(initializer != nullptr) {
    initializer->emitBytecode(chunk);
  }

  int conditionStart = chunk.bytecode.size();
  if(condition != nullptr) {
    condition->emitBytecode(chunk);
  }
  int conditionLength = chunk.bytecode.size() - conditionStart;

  chunk.emitByte(OP_JUMP_FALSE);
  
  int bodyStart = chunk.bytecode.size();
  body->emitBytecode(chunk);
  int bodyLength = chunk.bytecode.size() - bodyStart;

  int incrementorStart = chunk.bytecode.size();
  if(incrementor != nullptr) {
    incrementor->emitBytecode(chunk);
    chunk.emitByte(OP_POP);
  }
  int incrementorLength = chunk.bytecode.size() - incrementorStart;

  // After condition loop, skip entire body, incrementor and 3 extra bytes
  int jump = bodyLength + incrementorLength + 3;
  uint8_t jumpLow = ((jump >> 0) & 0xFF);
  uint8_t jumpHigh = ((jump >> 8) & 0XFF);

  chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 1, jumpLow);
  chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 2, jumpHigh);

  // Insert OP_LOOP, skip entire body, condition and 3 extra bytes
  jump = bodyLength + conditionLength + incrementorLength + 3;
  jumpLow = ((jump >> 0) & 0xFF);
  jumpHigh = ((jump >> 8) & 0XFF);

  chunk.emitByte(OP_LOOP);
  chunk.emitByte(jumpLow);
  chunk.emitByte(jumpHigh);

  if(initializer != nullptr && initializer->getType() == StatementType::DECLARE_VARIABLE) {
    chunk.emitByte(OP_POP);
  }
}

}