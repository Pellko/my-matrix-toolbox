#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_FUNCTION_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_FUNCTION_STATEMENT_H_

#include "src/types/CompilerScope.hh"
#include "Statement.hh"

namespace mymatrixtoolbox {

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

  void setArguments(std::vector<Argument> arguments) {
    this->arguments = arguments;
  }

  std::string getName() {
    return name;
  }

  static std::vector<Argument> readArgumentList(ParserTool& parserTool);

 private:
  std::string name;
  Statement* block;
  CompilerScope* compilerScope; // This pointer is not managed by this class
  CompilerScope* parentScope; // This pointer is not managed by this class
  int functionIndex;
  int globalIndex;
  std::vector<Upvalue> upvalues;
  std::vector<Argument> arguments;
  bool isGlobal;
};

}

#endif
