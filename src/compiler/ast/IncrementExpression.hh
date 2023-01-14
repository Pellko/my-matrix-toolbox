#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_INCREMENT_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_INCREMENT_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class IncrementExpression : public Expression {
 public:
  IncrementExpression(bool isIncrement, Expression* expression) : isIncrement(isIncrement), expression(expression) {}
  ~IncrementExpression() {
    if(expression != nullptr) delete expression;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  bool isIncrement;
  Expression* expression;
};

}

#endif