#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_IF_STATEMENT_H_

#include "Expression.hh"
#include "Statement.hh"

namespace mymatrixtoolbox {

class LambdaExpression : public Expression {
 public:
  LambdaExpression(std::shared_ptr<Statement> block, CompilerScope* compilerScope) : block(block), compilerScope(compilerScope) {}
  ~LambdaExpression() {}

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
  std::shared_ptr<Statement> block;
  CompilerScope* compilerScope;
  CompilerScope* parentScope;
  int functionIndex;
  std::vector<Upvalue> upvalues;
  std::vector<Argument> arguments;
};

}

#endif