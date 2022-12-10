#ifndef _SCISCRIPT_COMPILER_COMPILER_H_
#define _SCISCRIPT_COMPILER_COMPILER_H_

#include <vector>
#include <ctype.h>
#include "Lexer.hh"
#include "src/types/CompilerOutput.hh"
#include "Expression.hh"
#include "Statement.hh"
#include "SyntaxException.hh"

namespace sciscript {

class Compiler {
 public:
  Compiler(std::vector<Token>& tokens);
  ~Compiler() {}

  CompilerOutput compile();

 private:
  std::vector<Token>& tokens;
  int position;

  Token* peek(int n=0);
  Token* get();
  bool empty();

  void statement(CompilerOutput& output);  
  void expression(CompilerOutput& output);

  Statement* readStatement();
  Expression* readExpression(bool fromGroup = false);
  Expression* readTerm();
  std::variant<Expression*, PrimaryExpression*> readFactor();
  PrimaryExpression* readPrimary();
};

}

#endif