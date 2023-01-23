#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_WHILE_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_WHILE_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class WhileStatement : public Statement {
 public:
  WhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> body) : Statement(StatementType::WHILE), condition(condition), body(body) {}
  ~WhileStatement() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Statement> body;
};

}

#endif
