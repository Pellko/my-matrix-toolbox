#include "PrimaryExpression.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void ConstantExpression::emitBytecode(Chunk& chunk) {
  chunk.literals.push_back(literal);
  chunk.emitByte(OP_CONSTANT);
  chunk.emitDynamicBytes(chunk.literals.size() - 1);
}

void GroupExpression::emitBytecode(Chunk& chunk) {
  expression->emitBytecode(chunk);
}

void LocalExpression::emitBytecode(Chunk& chunk) {
  switch(type) {
    case VarRefType::GLOBAL:
      chunk.emitByte(OP_READ_GLOBAL);
      chunk.emitDynamicBytes(index);
      break;
    case VarRefType::LOCAL:
      chunk.emitByte(OP_READ_LOCAL);
      chunk.emitDynamicBytes(index);
      break;
    case VarRefType::UPVALUE:
      chunk.emitByte(OP_READ_UPVALUE);
      chunk.emitDynamicBytes(index);
      break;
    default:
      break;
  }
}

}