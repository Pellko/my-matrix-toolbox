#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_UNARY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_UNARY_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

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
