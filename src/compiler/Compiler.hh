#ifndef _SCISCRIPT_COMPILER_COMPILER_H_
#define _SCISCRIPT_COMPILER_COMPILER_H_

#include <vector>
#include "src/types/CompilerScope.hh"
#include "src/types/CompilerOutput.hh"
#include "ast/Statement.hh"
#include "ast/FunctionStatement.hh"
#include "Lexer.hh"
#include "ParserTool.hh"

namespace sciscript {

class Compiler {
 public:
  Compiler(std::vector<Token>& tokens) : parserTool(tokens) {}
  ~Compiler() {}

  void compile(CompilerOutput& compilerOutput);
  int declareNativeFunction(std::string name);

 private:
  ParserTool parserTool;
  CompilerScope* rootScope;

  void statement();
};

}

#endif