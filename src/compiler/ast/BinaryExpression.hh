#ifndef _SCISCRIPT_COMPILER_AST_BINARY_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_BINARY_EXPRESSION_H_

#include "Expression.hh"

namespace sciscript {

enum class BinaryOperation {
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  MODULUS,
  EQUALITY,
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
