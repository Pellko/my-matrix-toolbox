#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_MATRIX_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_MATRIX_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class AssignMatrixExpression : public Expression {
 public:
  AssignMatrixExpression(std::shared_ptr<Expression> target, std::shared_ptr<Expression> value, std::shared_ptr<Expression> row, std::shared_ptr<Expression> col) : target(target), value(value), row(row), col(col) {}
  ~AssignMatrixExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::shared_ptr<Expression> target;
  std::shared_ptr<Expression> value;
  std::shared_ptr<Expression> row;
  std::shared_ptr<Expression> col;
};

}

#endif
