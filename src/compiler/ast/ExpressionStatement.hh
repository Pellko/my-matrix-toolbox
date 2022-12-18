#ifndef _SCISCRIPT_COMPILER_AST_EXPRESSION_STATEMENT_H_
#define _SCISCRIPT_COMPILER_AST_EXPRESSION_STATEMENT_H_

#include "Statement.hh"
#include "Expression.hh"

namespace sciscript {

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