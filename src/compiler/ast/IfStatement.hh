#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class IfStatement : public Statement {
 public:
  IfStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> trueStatement) : Statement(StatementType::IF), condition(condition), trueStatement(trueStatement) {}
  ~IfStatement() {}

  void emitBytecode(Chunk& chunk) override;

  void addElifStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement) {
    elifStatements.push_back(std::make_pair(condition, statement));
  }

  void setElseStatement(std::shared_ptr<Statement> statement) {
    elseStatement = statement;
  }

 private:
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Statement> trueStatement;
  std::shared_ptr<Statement> elseStatement;
  std::vector<std::pair<std::shared_ptr<Expression>, std::shared_ptr<Statement>>> elifStatements;
};

}

#endif
