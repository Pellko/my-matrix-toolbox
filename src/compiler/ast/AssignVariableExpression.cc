#include "AssignVariableExpression.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void AssignVariableExpression::emitBytecode(Chunk& chunk) {
  if(type == DeclareVariableType::GLOBAL) {
    value->emitBytecode(chunk);
    chunk.emitByte(OP_SET_GLOBAL);
    chunk.emitDynamicBytes(index);
  } else {
    value->emitBytecode(chunk);
    chunk.emitByte(isUpvalue ? OP_SET_UPVALUE : OP_SET_LOCAL);
    chunk.emitDynamicBytes(index);
  }
}


}