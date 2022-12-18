#include "BinaryExpression.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void BinaryExpression::emitBytecode(Chunk& chunk) {
  left->emitBytecode(chunk);
  right->emitBytecode(chunk);
  
  switch(op) {
    case BinaryOperation::ADDITION:
      chunk.emitByte(OP_ADD);
      break;
    case BinaryOperation::SUBTRACTION:
      chunk.emitByte(OP_MINUS);    
      break;
    case BinaryOperation::MULTIPLICATION:
      chunk.emitByte(OP_MULT);
      break;
    case BinaryOperation::DIVISION:
      chunk.emitByte(OP_DIV);
      break;
    case BinaryOperation::MODULUS:
      chunk.emitByte(OP_MOD);
      break;
  }
}

}