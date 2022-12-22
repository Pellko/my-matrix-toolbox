#include "LambdaExpression.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void LambdaExpression::emitBytecode(Chunk& chunk) {
  block->emitBytecode(compilerScope->chunk);

  Chunk* parentChunk = nullptr;
  if(parentScope != nullptr) {
    parentChunk = &parentScope->chunk;
  } else {
    parentChunk = &chunk;
  }

  parentChunk->emitByte(OP_CLOSURE);
  parentChunk->emitDynamicBytes(functionIndex);

  for(const Upvalue& upvalue : upvalues) {
    parentChunk->emitByte(upvalue.isLocal ? 1 : 0);
    parentChunk->emitDynamicBytes(upvalue.index);
  }
}

}