#ifndef _SCISCRIPT_COMPILER_STATEMENT_H_
#define _SCISCRIPT_COMPILER_STATEMENT_H_

#include "src/types/CompilerOutput.hh"
#include "Expression.hh"

namespace sciscript {

struct CompilerOutput;

enum class StatementType {
  DECLARE_GLOBAL,
  DECLARE_LOCAL,
  PRINT,
  BLOCK,
  EXPRESSION,
};

class Statement {
 public:
  Statement(StatementType type) : type(type) {}
  ~Statement();

  StatementType type;

  struct SetGlobal {
    std::string name;
    Expression* value;
  };
  
  struct SetLocal {
    std::string name;
    Expression* value;
    int index;
  };

  struct Print {
    Expression* expression;
  };

  struct ExpressionStatement {
    Expression* expression;
  };

  struct Block {
    std::vector<Statement*> statements;
    int depth;

    int numLocals() {
      int num = 0;
      for(Statement* stmt : statements) {
        if(stmt->type == StatementType::DECLARE_LOCAL) {
          num++;
        }
      }
      return num;
    }
  };

  SetGlobal* declareGlobal;
  SetLocal* declareLocal;
  Print* print;
  Block* block;
  ExpressionStatement* expression;

  void emitBytecode(CompilerOutput& output);
};

}

#endif