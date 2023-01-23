#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_DECLARE_VARIABLE_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_DECLARE_VARIABLE_STATEMENT_H_

#include "Statement.hh"

namespace mymatrixtoolbox {

enum class DeclareVariableType {
  LOCAL,
  GLOBAL,
};

class DeclareVariableStatement : public Statement {
 public:
  DeclareVariableStatement(DeclareVariableType variableType, int index, std::shared_ptr<Expression> value) : Statement(StatementType::DECLARE_VARIABLE), variableType(variableType), index(index), value(value) {}
  ~DeclareVariableStatement() {}

  void emitBytecode(Chunk& chunk) override;

  DeclareVariableType getVariableType() {
    return variableType;
  }

 private:
  DeclareVariableType variableType;
  int index;
  std::shared_ptr<Expression> value;
};

}

#endif
