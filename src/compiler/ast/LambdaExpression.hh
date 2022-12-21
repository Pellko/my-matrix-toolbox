#ifndef _SCISCRIPT_COMPILER_AST_IF_STATEMENT_H_
#define _SCISCRIPT_COMPILER_AST_IF_STATEMENT_H_

#include "Expression.hh"
#include "Statement.hh"

namespace sciscript {

class LambdaExpression : public Expression {
 public:
  LambdaExpression(Statement* block, CompilerScope* compilerScope) : block(block), compilerScope(compilerScope) {}
  ~LambdaExpression() {
    if(block != nullptr) delete block;
  }

  void emitBytecode(Chunk& chunk) override;
  
  void setParentScope(CompilerScope* scope) {
    parentScope = scope;
  }

  void setFunctionIndex(int index) {
    functionIndex = index;
  }

  void addUpvalue(Upvalue value) {
    upvalues.push_back(value);
  }

  void setArguments(std::vector<Argument> arguments) {
    this->arguments = arguments;
  }

 private:
  Statement* block;
  CompilerScope* compilerScope;
  CompilerScope* parentScope;
  int functionIndex;
  std::vector<Upvalue> upvalues;
  std::vector<Argument> arguments;
};

}

#endif