#ifndef _MY_MATRIX_TOOLBOX_COMPILER_PARSER_TOOL_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_PARSER_TOOL_H_

#include "src/types/CompilerScope.hh"
#include "src/types/Local.hh"
#include "src/types/Object.hh"
#include "Lexer.hh"
#include <vector>
#include <unordered_map>
#include <memory>

namespace mymatrixtoolbox {

class BlockStatement;

struct CompilerScopeTree {
  CompilerScope* current;
  CompilerScopeTree* parent;
  int depth;
  int localsOffset;

  void free() {
    if(parent != nullptr) {
      parent->free();
      delete parent;
    }
  }
};

class ParserTool {
 public:
  ParserTool(std::vector<Token>& tokens) : tokens(tokens) {}
  ~ParserTool() {
    if(compilerScopeTree != nullptr) {
      compilerScopeTree->free();
      delete compilerScopeTree;
    }
  }

  Token* peek(int n=0);
  Token* get();
  bool empty();
  bool require(int n);
  bool expressionNext();

  void beginScope();
  void endScope();
  int getScopeLevel() {
    return scopeLevel;
  }

  void beginLambda();
  void beginFunction(std::string name);
  void endFunction();
  CompilerScope* currentScope() {
    if(compilerScopeTree == nullptr) {
      return nullptr;
    }
    return compilerScopeTree->current;
  }
  std::vector<CompilerScope*>& getFunctions() {
    return functionCompilerScopes;
  }

  std::pair<VarRefType, int> findIdentifier(std::string name);
  std::vector<Local>& getLocals() {
    return locals;
  }

  std::vector<std::string>& getGlobals() {
    return globals;
  }

  int registerGlobal(std::string name);
  int registerLocal(std::string name);
  int registerUpvalue(std::string name);

  void storeLocalsInBlockStatement(BlockStatement* statement);

 private:
  std::vector<Token>& tokens;
  std::vector<Local> locals;
  std::vector<std::string> globals;
  std::vector<CompilerScope*> functionCompilerScopes;
  CompilerScopeTree* compilerScopeTree = nullptr;

  int resolveUpvalue(CompilerScopeTree* scope, std::string name);
  int addUpvalue(CompilerScopeTree* scope, int index, bool isLocal);

  int scopeLevel = 0;
  int position = 0;
};

}

#endif