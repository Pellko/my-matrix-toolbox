#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_BINARY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_BINARY_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

enum class BinaryOperation {
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  MODULUS,
  EQUALITY,
  LESS_THAN,
  LESS_THAN_EQUALS,
  GREATER_THAN,
  GREATER_THAN_EQUALS,
};

class BinaryExpression : public Expression {
 public:
  BinaryExpression(BinaryOperation op, Expression* left, Expression* right) : op(op), left(left), right(right) {}
  ~BinaryExpression() {
    delete left;
    delete right;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  BinaryOperation op;
  Expression* left;
  Expression* right;
};

}

#endif
