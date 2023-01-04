#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

class IfStatement : public Statement {
 public:
  IfStatement(Expression* condition, Statement* trueStatement) : Statement(StatementType::IF), condition(condition), trueStatement(trueStatement) {}
  ~IfStatement() {
    delete condition;
    delete trueStatement;
    for(auto stmt : elifStatements) {
      delete stmt.first;
      delete stmt.second;
    }
    if(elseStatement != nullptr) {
      delete elseStatement;
    }
  }

  void emitBytecode(Chunk& chunk) override;

  void addElifStatement(Expression* condition, Statement* statement) {
    elifStatements.push_back(std::make_pair(condition, statement));
  }

  void setElseStatement(Statement* statement) {
    elseStatement = statement;
  }

 private:
  Expression* condition;
  Statement* trueStatement;
  Statement* elseStatement;
  std::vector<std::pair<Expression*, Statement*>> elifStatements;
};

}

#endif
