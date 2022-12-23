#include "AssignPropertyExpression.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void AssignPropertyExpression::emitBytecode(Chunk& chunk) {
  value->emitBytecode(chunk);
  target->emitBytecode(chunk);
  int literalId = chunk.registerStringLiteral(propertyName);
  chunk.emitByte(OP_SET_PROPERTY);
  chunk.emitDynamicBytes(literalId);
}

}