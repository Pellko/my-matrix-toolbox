#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_FOR_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_FOR_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class ForStatement : public Statement {
 public:
  ForStatement() : Statement(StatementType::FOR) {}
  ~ForStatement() {
    if(initializer != nullptr) delete initializer;
    if(condition != nullptr) delete condition;
    if(incrementor != nullptr) delete incrementor;
    if(body != nullptr) delete body; 
  }

  void emitBytecode(Chunk& chunk) override;

  void setInitializer(Statement* stmt) {
    initializer = stmt;
  }

  void setCondition(Expression* expression) {
    condition = expression;
  }

  void setIncrementor(Expression* expression) {
    incrementor = expression;
  }

  void setBody(Statement* body) {
    this->body = body;
  }

 private:
  Statement* initializer;
  Expression* condition;
  Expression* incrementor;
  Statement* body;
};

}

#endif

