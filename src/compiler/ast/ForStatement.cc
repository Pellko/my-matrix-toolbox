#include "ForStatement.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

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
  chunk.emitByte(OP_LOOP);

  int jumpSize = conditionLength + bodyLength + incrementorLength;
  std::vector<uint8_t> bytes = code::generateDynamicBytes(jumpSize);
  jumpSize += bytes.size();
  if(code::generateDynamicBytes(jumpSize).size() != bytes.size()) {
    int oldSize = bytes.size();
    bytes = code::generateDynamicBytes(jumpSize);
    jumpSize -= oldSize;
    jumpSize += bytes.size();
  } else {
    bytes = code::generateDynamicBytes(jumpSize);
  }
  chunk.emitDynamicBytes(jumpSize);
  for(int i=0;i<bytes.size();i++) {
    chunk.bytecode.insert(chunk.bytecode.begin() + conditionStart + conditionLength + 1 + i, bytes[i]);
  }
  if(initializer != nullptr && initializer->getType() == StatementType::DECLARE_VARIABLE) {
    chunk.emitByte(OP_POP);
  }
}

}