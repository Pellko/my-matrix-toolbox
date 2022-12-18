#ifndef _SCISCRIPT_COMPILER_AST_UNARY_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_UNARY_EXPRESSION_H_

#include "Expression.hh"

namespace sciscript {

enum class UnaryOperation {
  NEGATION,
};

class UnaryExpression : public Expression {
 public:
  UnaryExpression(UnaryOperation op, Expression* expression) : op(op), expression(expression) {}
  ~UnaryExpression() {
    if(expression != nullptr) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  UnaryOperation op;
  Expression* expression;
};

}

#endif
