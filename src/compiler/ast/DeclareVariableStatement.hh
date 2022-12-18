#ifndef _SCISCRIPT_COMPILER_AST_DECLARE_VARIABLE_STATEMENT_H_
#define _SCISCRIPT_COMPILER_AST_DECLARE_VARIABLE_STATEMENT_H_

#include "Statement.hh"

namespace sciscript {

enum class DeclareVariableType {
  LOCAL,
  GLOBAL,
};

class DeclareVariableStatement : public Statement {
 public:
  DeclareVariableStatement(DeclareVariableType variableType, int index, Expression* value) : Statement(StatementType::DECLARE_VARIABLE), variableType(variableType), index(index), value(value) {}
  ~DeclareVariableStatement() {
    if(value != nullptr) {
      delete value;
    }
  }

  void emitBytecode(Chunk& chunk) override;

  DeclareVariableType getVariableType() {
    return variableType;
  }

 private:
  DeclareVariableType variableType;
  int index;
  Expression* value;
};

}

#endif
