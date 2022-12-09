#include "Expression.hh"
#include "Compiler.hh"

namespace sciscript {

PrimaryExpression::~PrimaryExpression() {
  delete group;
}

void PrimaryExpression::emitBytecode(CompilerOutput& output) {
  if(type == PrimaryType::CONSTANT) {
    output.pushConstant(value);
  } else if (type == PrimaryType::GROUP) {
    group->emitBytecode(output);
  }
}

Expression::~Expression() {
  if(arithmetic != nullptr) {
    delete arithmetic->left;
    delete arithmetic->right;
    delete arithmetic;
  }
  if(negation != nullptr) {
    delete negation->child;
    delete negation;
  }
  delete primary;
}

void Expression::emitBytecode(CompilerOutput& output) {
  if(type == ExpressionType::ARITHMETIC) {
    arithmetic->left->emitBytecode(output);
    arithmetic->right->emitBytecode(output);
    switch(arithmetic->type) {
      case ArithmeticType::PLUS:
        output.emitByte(OP_ADD);
        break;
      case ArithmeticType::MINUS:
        output.emitByte(OP_MINUS);
        break;
      case ArithmeticType::MULT:
        output.emitByte(OP_MULT);
        break;
      case ArithmeticType::DIV:
        output.emitByte(OP_DIV);
        break;
      case ArithmeticType::MOD:
        output.emitByte(OP_MOD);
        break;
    }
  }

  if(type == ExpressionType::NEGATION) {
    negation->child->emitBytecode(output);
    output.emitByte(OP_NEG);
  }

  if(type == ExpressionType::PRIMARY) {
    primary->emitBytecode(output);
  }
}

}