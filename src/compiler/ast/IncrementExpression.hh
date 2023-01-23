#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_INCREMENT_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_INCREMENT_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class IncrementExpression : public Expression {
 public:
  IncrementExpression(bool isIncrement, std::shared_ptr<Expression> expression, std::shared_ptr<Expression> value) : isIncrement(isIncrement), expression(expression), value(value) {}
  ~IncrementExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  bool isIncrement;
  std::shared_ptr<Expression> expression;
  std::shared_ptr<Expression> value;
};

}

#endif