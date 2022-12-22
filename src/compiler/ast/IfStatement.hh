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
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* condition;
  Statement* trueStatement;
};

}

#endif
