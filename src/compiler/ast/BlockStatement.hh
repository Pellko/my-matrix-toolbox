#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_BLOCK_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_BLOCK_STATEMENT_H_

#include "Statement.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"

namespace mymatrixtoolbox {

class BlockStatement : public Statement {
 public:
  BlockStatement() : Statement(StatementType::BLOCK) {}
  ~BlockStatement() {}

  void addStatement(std::shared_ptr<Statement> statement) {
    statements.push_back(statement);
  }

  void addLocal(Local local) {
    locals.push_back(local);
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  std::vector<std::shared_ptr<Statement>> statements;
  std::vector<Local> locals;
};

}

#endif
