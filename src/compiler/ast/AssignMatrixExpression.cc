#include "AssignMatrixExpression.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void AssignMatrixExpression::emitBytecode(Chunk& chunk) {
  value->emitBytecode(chunk);
  target->emitBytecode(chunk);
  row->emitBytecode(chunk);
  col->emitBytecode(chunk);
  chunk.emitByte(OP_SET_MATRIX);
}

}