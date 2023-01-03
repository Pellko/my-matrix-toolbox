#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_MATRIX_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_MATRIX_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class AssignMatrixExpression : public Expression {
 public:
  AssignMatrixExpression(Expression* target, Expression* value, Expression* row, Expression* col) : target(target), value(value), row(row), col(col) {}
  ~AssignMatrixExpression() {
    if(target != nullptr) delete target;
    if(value != nullptr) delete value;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* target;
  Expression* value;
  Expression* row;
  Expression* col;
};

}

#endif
