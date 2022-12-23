#include "DeclareClassStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void DeclareClassStatement::emitBytecode(Chunk& chunk) {
  chunk.emitByte(OP_CLASS);

  for(ClassMethod& method : methods) {
    // Emit function body
    method.block->emitBytecode(method.compilerScope->chunk);

    // Emit closure
    chunk.emitByte(OP_CLOSURE);
    chunk.emitDynamicBytes(method.functionIndex);

    for(const Upvalue& upvalue : method.upvalues) {
      chunk.emitByte(upvalue.isLocal ? 1 : 0);
      chunk.emitDynamicBytes(upvalue.index);
    }

    // Create method on class
    int literalId = chunk.registerStringLiteral(method.name);
    chunk.emitByte(OP_METHOD);
    chunk.emitDynamicBytes(literalId);
  }

  if(isGlobal) {
    chunk.emitByte(OP_SET_GLOBAL);
    chunk.emitDynamicBytes(classIndex);
    chunk.emitByte(OP_POP);
  }
}

}