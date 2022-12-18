#include "UnaryExpression.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void UnaryExpression::emitBytecode(Chunk& chunk) {
  expression->emitBytecode(chunk);

  switch(op) {
    case UnaryOperation::NEGATION:
      chunk.emitByte(OP_NEG);
      break;
  }
}

}