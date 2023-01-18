#ifndef _MY_MATRIX_TOOLBOX_COMPILER_COMPILER_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_COMPILER_H_

#include <vector>
#include "src/types/CompilerScope.hh"
#include "src/types/CompilerOutput.hh"
#include "ast/Statement.hh"
#include "ast/FunctionStatement.hh"
#include "Lexer.hh"
#include "ParserTool.hh"

namespace mymatrixtoolbox {

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
  void compileModule(std::string name, CompilerOutput& compilerOutput);
  std::string readFile(std::string fileName);
};

}

#endif