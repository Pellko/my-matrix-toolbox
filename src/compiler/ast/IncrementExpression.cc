#include "IncrementExpression.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void IncrementExpression::emitBytecode(Chunk& chunk) {
  // chunk.literals.push_back(Literal::fromDouble(1));
  // chunk.emitByte(OP_CONSTANT);
  // chunk.emitDynamicBytes(chunk.literals.size() - 1);
  value->emitBytecode(chunk);
  expression->emitBytecode(chunk);
  chunk.emitByte(isIncrement ? OP_ADD : OP_MINUS);
}

}