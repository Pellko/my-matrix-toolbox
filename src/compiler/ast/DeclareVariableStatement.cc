#include "DeclareVariableStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void DeclareVariableStatement::emitBytecode(Chunk& chunk) {
  if(variableType == DeclareVariableType::GLOBAL) {
    value->emitBytecode(chunk);
    chunk.emitByte(OP_SET_GLOBAL);
    chunk.emitDynamicBytes(index);
    chunk.emitByte(OP_POP);
  } else {
    value->emitBytecode(chunk);
  }
}

}