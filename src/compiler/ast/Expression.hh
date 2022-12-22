#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_EXPRESSION_H_

#include "src/types/Chunk.hh"
#include "src/compiler/ParserTool.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/Lexer.hh"

namespace mymatrixtoolbox {

class Expression {
 public:
  Expression() {}
  virtual ~Expression() {}

  static Expression* parse(ParserTool& parserToolfalse);
  virtual void emitBytecode(Chunk& chunk) = 0;

 private:
  static Expression* readPrimary(ParserTool& parserTool);
  static Expression* readFactor(ParserTool& parserTool);
  static Expression* readTerm(ParserTool& parserTool);
  static Expression* readArithmeticExpression(ParserTool& parserTool);
};

}

#endif
