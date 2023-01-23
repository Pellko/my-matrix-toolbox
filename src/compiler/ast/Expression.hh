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

  static std::shared_ptr<Expression> parse(ParserTool& parserToolfalse);
  virtual void emitBytecode(Chunk& chunk) = 0;

 private:
  static std::shared_ptr<Expression> readPrimary(ParserTool& parserTool);
  static std::shared_ptr<Expression> readFactor(ParserTool& parserTool);
  static std::shared_ptr<Expression> readTerm(ParserTool& parserTool);
  static std::shared_ptr<Expression> readArithmeticExpression(ParserTool& parserTool);

  static std::shared_ptr<Expression> readCall(ParserTool& parserTool, std::shared_ptr<Expression> target);
  static std::shared_ptr<Expression> readAssignment(ParserTool& parserTool, std::string identifier, std::function<std::shared_ptr<Expression> (std::shared_ptr<Expression> value)> value);
  static std::shared_ptr<Expression> readIdentifier(ParserTool& parserTool, std::string identifier);
};

}

#endif
