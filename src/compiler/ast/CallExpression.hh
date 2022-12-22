#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_CALL_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_CALL_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class CallExpression : public Expression {
 public:
  CallExpression(Expression* target, std::vector<Expression*> arguments) : target(target), arguments(arguments) {}
  ~CallExpression() {
    if(target != nullptr) {
      delete target;
    }
    for(Expression* arg : arguments) {
      delete arg;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* target;
  std::vector<Expression*> arguments;
};

class MatrixAccessExpression : public Expression {
 public:
  MatrixAccessExpression(Expression* target, Expression* row, Expression* col) : target(target), row(row), col(col) {}
  ~MatrixAccessExpression() {
    if(target != nullptr) delete target;
    if(row != nullptr) delete row;
    if(col != nullptr) delete col;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* target;
  Expression* row;
  Expression* col;
};

}

#endif