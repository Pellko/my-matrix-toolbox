#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_PRINT_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_PRINT_STATEMENT_H_

#include "Statement.hh"
#include "Expression.hh"

namespace mymatrixtoolbox {

class PrintStatement : public Statement {
 public:
  PrintStatement(Expression* expression) : Statement(StatementType::PRINT), expression(expression) {}
  ~PrintStatement() {
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
