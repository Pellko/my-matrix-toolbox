#include "CallExpression.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void CallExpression::emitBytecode(Chunk& chunk) {
  target->emitBytecode(chunk);
  for(Expression* arg : arguments) {
    arg->emitBytecode(chunk);
  }
  chunk.emitByte(OP_CALL);
  chunk.emitDynamicBytes(arguments.size());
}

void MatrixAccessExpression::emitBytecode(Chunk& chunk) {
  target->emitBytecode(chunk);
  row->emitBytecode(chunk);
  col->emitBytecode(chunk);
  chunk.emitByte(OP_READ_MATRIX);
}

}