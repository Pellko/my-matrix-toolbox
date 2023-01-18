#include "ImportModuleStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void ImportModuleStatement::emitBytecode(Chunk& chunk) {
  if(variableType == DeclareVariableType::GLOBAL) {
    chunk.emitByte(OP_MODULE);
    chunk.emitDynamicBytes(moduleId);
    chunk.emitByte(OP_SET_GLOBAL);
    chunk.emitDynamicBytes(index);
    chunk.emitByte(OP_POP);
  } else {
    chunk.emitByte(OP_MODULE);
    chunk.emitDynamicBytes(moduleId);
  }
}

}