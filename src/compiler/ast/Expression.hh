#ifndef _SCISCRIPT_COMPILER_AST_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_EXPRESSION_H_

#include "src/types/Chunk.hh"
#include "src/compiler/ParserTool.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/Lexer.hh"

namespace sciscript {

class Expression {
 public:
  Expression() {}
  virtual ~Expression() {}

  static Expression* parse(ParserTool& parserTool, bool fromGroup = false);
  virtual void emitBytecode(Chunk& chunk) = 0;

 private:
  static Expression* readPrimary(ParserTool& parserTool);
  static Expression* readFactor(ParserTool& parserTool);
  static Expression* readTerm(ParserTool& parserTool);
};

}

#endif
