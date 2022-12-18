#include "ExpressionStatement.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void ExpressionStatement::emitBytecode(Chunk& chunk) {
  expression->emitBytecode(chunk);
  chunk.emitByte(OP_POP);
}

}