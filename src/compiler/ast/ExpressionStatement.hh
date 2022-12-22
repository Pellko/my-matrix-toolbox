#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_EXPRESSION_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_EXPRESSION_STATEMENT_H_

#include "Statement.hh"
#include "Expression.hh"

namespace mymatrixtoolbox {

class ExpressionStatement : public Statement {
 public:
  ExpressionStatement(Expression* expression) : Statement(StatementType::EXPRESSION), expression(expression) {}
  ~ExpressionStatement() {
    if(expression != nullptr) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* expression;
};

}

#endif