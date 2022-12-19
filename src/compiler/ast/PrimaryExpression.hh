#ifndef _SCISCRIPT_COMPILER_AST_PRIMARY_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_PRIMARY_EXPRESSION_H_

#include "src/types/Literal.hh"
#include "Expression.hh"

namespace sciscript {

class ConstantExpression : public Expression {
 public:
  ConstantExpression(Literal literal) : literal(literal) {}
  ~ConstantExpression() {}

  void emitBytecode(Chunk& chunk) override;
  
 private:
  Literal literal;
};

class GroupExpression : public Expression {
 public:
  GroupExpression(Expression* expression) : expression(expression) {}
  ~GroupExpression() {
    if(expression != nullptr) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* expression;
};

class LocalExpression : public Expression {
 public:
  LocalExpression(VarRefType type, int index) : type(type), index(index) {}

  void emitBytecode(Chunk& chunk) override;

 private:
  VarRefType type;
  int index;
};

}

#endif