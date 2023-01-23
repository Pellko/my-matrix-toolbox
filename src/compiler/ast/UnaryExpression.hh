#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_UNARY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_UNARY_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

enum class UnaryOperation {
  NEGATION,
};

class UnaryExpression : public Expression {
 public:
  UnaryExpression(UnaryOperation op, std::shared_ptr<Expression> expression) : op(op), expression(expression) {}
  ~UnaryExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  UnaryOperation op;
  std::shared_ptr<Expression> expression;
};

}

#endif
