#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_CALL_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_CALL_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class CallExpression : public Expression {
 public:
  CallExpression(std::shared_ptr<Expression> target, std::vector<std::shared_ptr<Expression>> arguments) : target(target), arguments(arguments) {}
  ~CallExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::shared_ptr<Expression> target;
  std::vector<std::shared_ptr<Expression>> arguments;
};

class MatrixAccessExpression : public Expression {
 public:
  MatrixAccessExpression(std::shared_ptr<Expression> target, std::shared_ptr<Expression> row, std::shared_ptr<Expression> col) : target(target), row(row), col(col) {}
  ~MatrixAccessExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::shared_ptr<Expression> target;
  std::shared_ptr<Expression> row;
  std::shared_ptr<Expression> col;
};

}

#endif