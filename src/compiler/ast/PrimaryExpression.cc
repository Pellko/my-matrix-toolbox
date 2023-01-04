#include "PrimaryExpression.hh"
#include "src/types/Chunk.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void ConstantExpression::emitBytecode(Chunk& chunk) {
  if(literal.type == LiteralType::STRING) {
    int idx = chunk.registerStringLiteral(*literal.as.str);
    chunk.emitByte(OP_CONSTANT);
    chunk.emitDynamicBytes(idx);
    literal.free();
  } else {
    chunk.literals.push_back(literal);
    chunk.emitByte(OP_CONSTANT);
    chunk.emitDynamicBytes(chunk.literals.size() - 1);
  }
}

void MapExpression::emitBytecode(Chunk& chunk) {
  for(auto value : values) {
    value.first->emitBytecode(chunk);
    value.second->emitBytecode(chunk);
  }
  chunk.emitByte(OP_MAP);
  chunk.emitDynamicBytes(values.size());
}

void MatrixExpression::emitBytecode(Chunk& chunk) {
  for(Expression* expression : expressions) {
    expression->emitBytecode(chunk);
  }
  chunk.emitByte(OP_MATRIX);
  chunk.emitDynamicBytes(width);
  chunk.emitDynamicBytes(height);
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

void ReadPropertyExpression::emitBytecode(Chunk& chunk) {
  target->emitBytecode(chunk);
  int literalId = chunk.registerStringLiteral(propertyName);
  chunk.emitByte(OP_READ_PROPERTY);
  chunk.emitDynamicBytes(literalId);
}

}