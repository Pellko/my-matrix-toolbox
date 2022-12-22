#include "ExpressionStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void ExpressionStatement::emitBytecode(Chunk& chunk) {
  expression->emitBytecode(chunk);
  chunk.emitByte(OP_POP);
}

}