#include "CallExpression.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void CallExpression::emitBytecode(Chunk& chunk) {
  target->emitBytecode(chunk);
  for(Expression* arg : arguments) {
    arg->emitBytecode(chunk);
  }
  chunk.emitByte(OP_CALL);
  chunk.emitDynamicBytes(arguments.size());
}

}