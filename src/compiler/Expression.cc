#include "Expression.hh"

namespace sciscript {

PrimaryExpression::~PrimaryExpression() {
  delete group;
}

void PrimaryExpression::emitBytecode(CompilerOutput& output) {
  switch(type) {
    case PrimaryType::CONSTANT:
      output.pushConstant(value);
      break;
    case PrimaryType::GROUP:
      group->emitBytecode(output);
      break;
    case PrimaryType::GLOBAL_VARIABLE:
      output.readGlobal(globalVariableIndex);
      break;
    case PrimaryType::LOCAL_VARIABLE:
      output.readLocal(localVariableIndex);
      break;
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
  if(setLocal != nullptr) {
    delete setLocal->value;
    delete setLocal;
  }
  if(setGlobal != nullptr) {
    delete setGlobal->value;
    delete setGlobal;
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

  if(type == ExpressionType::SET_LOCAL) {
    setLocal->value->emitBytecode(output);
    output.setLocal(setLocal->name, setLocal->index);
  }

  if(type == ExpressionType::SET_GLOBAL) {
    setGlobal->value->emitBytecode(output);
    output.pushGlobal(setGlobal->name);
  }
}

}