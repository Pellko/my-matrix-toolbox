#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_RETURN_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_RETURN_STATEMENT_H_

#include "Statement.hh"
#include "Expression.hh"

namespace mymatrixtoolbox {

class ReturnStatement : public Statement {
 public:
  ReturnStatement() : Statement(StatementType::RETURN) {}
  ~ReturnStatement() {
    if(expression != nullptr) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;
  void setExpression(Expression* expression) {
    this->expression = expression;
  }

 private:
  Expression* expression = nullptr;
};

}

#endif