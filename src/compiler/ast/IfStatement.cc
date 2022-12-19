#include "IfStatement.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void IfStatement::emitBytecode(Chunk& chunk) {
  condition->emitBytecode(chunk);
  chunk.emitByte(OP_JUMP_FALSE);

  int trueStatementStart = chunk.bytecode.size();
  trueStatement->emitBytecode(chunk);
  int trueStatementLength = chunk.bytecode.size() - trueStatementStart;

  // Set OP_JUMP_FALSE argument
  std::vector<uint8_t> bytes = code::generateDynamicBytes(trueStatementLength);
  for(int i=0;i<bytes.size();i++) {
    chunk.bytecode.insert(chunk.bytecode.begin() + trueStatementStart + i, bytes[i]);
  }
}

}