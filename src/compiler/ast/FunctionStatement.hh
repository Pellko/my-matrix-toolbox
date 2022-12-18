#ifndef _SCISCRIPT_COMPILER_AST_FUNCTION_STATEMENT_H_
#define _SCISCRIPT_COMPILER_AST_FUNCTION_STATEMENT_H_

#include "src/types/CompilerScope.hh"
#include "Statement.hh"

namespace sciscript {

class FunctionStatement : public Statement {
 public:
  FunctionStatement(std::string name, bool isGlobal, Statement* block, CompilerScope* compilerScope) : Statement(StatementType::FUNCTION), name(name), block(block), compilerScope(compilerScope), isGlobal(isGlobal) {}
  ~FunctionStatement() {
    if(block != nullptr) {
      delete block;
    }
  }

  void emitBytecode(Chunk& chunk) override;

  void setParentScope(CompilerScope* parentScope) {
    this->parentScope = parentScope;
  }

  void setFunctionIndex(int index) {
    functionIndex = index;
  }

  void setGlobalIndex(int index) {
    globalIndex = index;
  }

  void addUpvalue(Upvalue value) {
    upvalues.push_back(value);
  }

  std::string getName() {
    return name;
  }

 private:
  std::string name;
  Statement* block;
  CompilerScope* compilerScope; // This pointer is not managed by this class
  CompilerScope* parentScope; // This pointer is not managed by this class
  int functionIndex;
  int globalIndex;
  std::vector<Upvalue> upvalues;
  bool isGlobal;
};

}

#endif
