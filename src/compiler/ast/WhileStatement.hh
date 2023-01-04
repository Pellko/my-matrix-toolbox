#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_WHILE_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_WHILE_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class WhileStatement : public Statement {
 public:
  WhileStatement(Expression* condition, Statement* body) : Statement(StatementType::WHILE), condition(condition), body(body) {}
  ~WhileStatement() {
    if(condition != nullptr) delete condition;
    if(body != nullptr) delete body;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* condition;
  Statement* body;
};

}

#endif
