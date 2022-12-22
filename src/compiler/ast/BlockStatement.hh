#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_BLOCK_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_BLOCK_STATEMENT_H_

#include "Statement.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"

namespace mymatrixtoolbox {

class BlockStatement : public Statement {
 public:
  BlockStatement(int depth) : Statement(StatementType::BLOCK), depth(depth) {}
  ~BlockStatement() {
    for(Statement* stmt : statements) {
      delete stmt;
    } 
  }

  void addStatement(Statement* statement) {
    statements.push_back(statement);
  }

  void addLocal(Local local) {
    locals.push_back(local);
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  int depth;
  std::vector<Statement*> statements;
  std::vector<Local> locals;
};

}

#endif
