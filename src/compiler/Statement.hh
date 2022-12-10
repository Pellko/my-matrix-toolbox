#ifndef _SCISCRIPT_COMPILER_STATEMENT_H_
#define _SCISCRIPT_COMPILER_STATEMENT_H_

#include "src/types/CompilerOutput.hh"
#include "Expression.hh"

namespace sciscript {

struct CompilerOutput;

enum class StatementType {
  DECLARE_GLOBAL,
  PRINT,
};

class Statement {
 public:
  Statement(StatementType type) : type(type) {}
  ~Statement();

  StatementType type;

  struct DeclareGlobal {
    std::string name;
    Expression* initializer;
  };

  struct Print {
    Expression* expression;
  };

  DeclareGlobal* declareGlobal;
  Print* print;

  void emitBytecode(CompilerOutput& output);
};

}

#endif