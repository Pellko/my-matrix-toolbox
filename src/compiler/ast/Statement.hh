#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_STATEMENT_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_STATEMENT_H_

#include "src/types/Chunk.hh"
#include "src/types/Argument.hh"
#include "src/compiler/ParserTool.hh"
#include "src/compiler/SyntaxException.hh"
#include "Expression.hh"

namespace mymatrixtoolbox {

enum class StatementType {
  BLOCK,
  FUNCTION,
  EXPRESSION,
  PRINT,
  DECLARE_VARIABLE,
  RETURN,
  IF,
  FOR,
  WHILE,
};

class Statement {
 public:
  Statement(StatementType type) : type(type) {}
  virtual ~Statement() {}

  static Statement* parse(ParserTool& parserTool);
  virtual void emitBytecode(Chunk& chunk) = 0;

  StatementType getType() {
    return type;
  }

 private:
  StatementType type;
  static std::vector<Statement*> readBlock(ParserTool& parserTool);
};

}

#endif
