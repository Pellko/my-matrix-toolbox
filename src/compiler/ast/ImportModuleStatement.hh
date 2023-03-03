#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_IMPORT_MODULE_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_IMPORT_MODULE_STATEMENT_H_

#include "Statement.hh"
#include "DeclareVariableStatement.hh"

namespace mymatrixtoolbox {

class ImportModuleStatement : public Statement {
 public:
  ImportModuleStatement(DeclareVariableType type, int index, int moduleId) : Statement(StatementType::IMPORT_MODULE), variableType(type), index(index), moduleId(moduleId) {}
  ~ImportModuleStatement() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  DeclareVariableType variableType;
  int index;
  int moduleId;
};

}

#endif