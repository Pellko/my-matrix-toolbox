#include "CallExpression.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void CallExpression::emitBytecode(Chunk& chunk) {
  target->emitBytecode(chunk);
  chunk.emitByte(OP_CALL);
  chunk.emitDynamicBytes(0); // numbers of arguments to function

}

}