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

  // if(initializer != nullptr) {
  //   initializer->emitBytecode(chunk);
  // }

  // int ARGUMENT_OFFSET = 2;
  // int conditionStart = chunk.bytecode.size();
  // if(condition != nullptr) {
  //   condition->emitBytecode(chunk);
  // }
  // //int conditionLength = chunk.bytecode.size() - conditionStart;
  // chunk.emitByte(OP_JUMP_FALSE);

  // int bodyStart = chunk.bytecode.size();
  // body->emitBytecode(chunk);

  // if(incrementor != nullptr) {
  //   incrementor->emitBytecode(chunk);
  //   chunk.emitByte(OP_POP);
  // }
  // chunk.emitByte(OP_LOOP);

  // int jump = chunk.bytecode.size() - conditionStart + ARGUMENT_OFFSET;
  // int jumpLow = ((jump >> 0) & 0xFF);
  // int jumpHigh = ((jump >> 8) & 0XFF);

  // chunk.emitByte(jumpLow);
  // chunk.emitByte(jumpHigh);
  
  // jump = chunk.bytecode.size() - bodyStart;
  // jumpLow = ((jump >> 0) & 0xFF);
  // jumpHigh = ((jump >> 8) & 0XFF);

  // chunk.bytecode.insert(chunk.bytecode.begin() + bodyStart, jumpLow);
  // chunk.bytecode.insert(chunk.bytecode.begin() + bodyStart + 1, jumpHigh);
  // // int conditionStart = chunk.bytecode.size();
  // // if(condition != nullptr) {
  // //   condition->emitBytecode(chunk);
  // // }
  // // int conditionLength = chunk.bytecode.size() - conditionStart;
  // // chunk.emitByte(OP_JUMP_FALSE);

  // // body->emitBytecode(chunk);
  // // if(incrementor != nullptr) {
  // //   incrementor->emitBytecode(chunk);
  // //   chunk.emitByte(OP_POP);
  // // }
  // // chunk.emitByte(OP_LOOP);

  // // int jumpSize = chunk.bytecode.size() - conditionStart + 1;
  // // std::vector<uint8_t> bytes = code::generateDynamicBytes(jumpSize);
  // // jumpSize += bytes.size() + 1;
  // // bytes = code::generateDynamicBytes(jumpSize);

  // // chunk.emitDynamicBytes(jumpSize);
  // // for(int i=0;i<bytes.size();i++) {
  // //   chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 1 + i, bytes[i]);
  // // }
  // // if(initializer != nullptr && initializer->getType() == StatementType::DECLARE_VARIABLE) {
  // //   chunk.emitByte(OP_POP);
  // // }
  // // int jumpSize = chunk.bytecode.size() - conditionStart + 1;
  // // std::vector<uint8_t> bytes = code::generateDynamicBytes(jumpSize);
  // // jumpSize += bytes.size();
  // // if(code::generateDynamicBytes(jumpSize).size() != bytes.size()) {
  // //   int oldSize = bytes.size();
  // //   bytes = code::generateDynamicBytes(jumpSize);
  // //   jumpSize -= oldSize;
  // //   jumpSize += bytes.size();
  // // } else {
  // //   bytes = code::generateDynamicBytes(jumpSize);
  // // }
  // // chunk.emitDynamicBytes(jumpSize - bytes.size());
  // // for(int i=0;i<bytes.size();i++) {
  // //   chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 1 + i, bytes[i]);
  // // }
  // if(initializer != nullptr && initializer->getType() == StatementType::DECLARE_VARIABLE) {
  //   chunk.emitByte(OP_POP);
  // }
}

}