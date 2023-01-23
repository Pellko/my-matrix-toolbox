#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_FOR_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_FOR_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class ForStatement : public Statement {
 public:
  ForStatement() : Statement(StatementType::FOR) {}
  ~ForStatement() {}

  void emitBytecode(Chunk& chunk) override;

  void setInitializer(std::shared_ptr<Statement> stmt) {
    initializer = stmt;
  }

  void setCondition(std::shared_ptr<Expression> expression) {
    condition = expression;
  }

  void setIncrementor(std::shared_ptr<Expression> expression) {
    incrementor = expression;
  }

  void setBody(std::shared_ptr<Statement> body) {
    this->body = body;
  }

 private:
  std::shared_ptr<Statement> initializer;
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Expression> incrementor;
  std::shared_ptr<Statement> body;
};

}

#endif

