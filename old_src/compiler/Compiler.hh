#ifndef _SCISCRIPT_COMPILER_COMPILER_H_
#define _SCISCRIPT_COMPILER_COMPILER_H_

#include <vector>
#include <ctype.h>
#include "src/types/CompilerOutput.hh"
#include "src/types/Local.hh"
#include "Lexer.hh"
#include "Expression.hh"
#include "Statement.hh"
#include "SyntaxException.hh"

namespace sciscript {

class Compiler {
 public:
  Compiler(std::vector<Token>& tokens);
  ~Compiler() {}

  CompilerOutput& compile();
  
 private:
  CompilerOutput output;
  std::vector<Token>& tokens;
  int position;
  int currScopeLevel = 0;
  std::vector<Local> locals;

  Token* peek(int n=0);
  Token* get();
  bool empty();
  bool require(int n);
  bool expressionNext();

  void statement(CompilerOutput& output);  
  void expression(CompilerOutput& output);

  Statement* readStatement();
  std::vector<Statement*> readBlock();
  Expression* readExpression(bool fromGroup = false);
  Expression* readTerm();
  std::variant<Expression*, PrimaryExpression*> readFactor();
  PrimaryExpression* readPrimary();
  Statement* declareGlobal(Token* variableName);
  Statement* declareLocal(Token* variableName);

  void beginScope();
  void endScope();
};

}

#endif