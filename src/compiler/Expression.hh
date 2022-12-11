#ifndef _SCISCRIPT_COMPILER_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_EXPRESSION_H_

#include "src/types/OpCode.hh"
#include "src/types/Value.hh"
#include "src/types/CompilerOutput.hh"
#include "src/types/ArithmeticType.hh"
#include <iostream>

namespace sciscript {

class PrimaryExpression;

enum class ExpressionType {
  ARITHMETIC,
  NEGATION,
  PRIMARY,
  COMPARISON,
  SET_LOCAL,
  SET_GLOBAL,
  TERNARY,
};

enum class PrimaryType {
  CONSTANT,
  GLOBAL_VARIABLE,
  LOCAL_VARIABLE,
  GROUP,
};

class Expression {
 public:
  Expression(ExpressionType type) : type(type) {}
  ~Expression();

  ExpressionType type;

  struct Arithmetic {
    ArithmeticType type;
    Expression* left;
    Expression* right;
  };

  struct Negation {
    PrimaryExpression* child;
  };

  struct Comparison {
    ComparisonType type;
    Expression* left;
    Expression* right;    
  };

  struct SetLocal {
    std::string name;
    Expression* value;
    int index;
  };

  struct SetGlobal {
    std::string name;
    Expression* value;
  };

  struct Ternary {
    Expression* condition;
    Expression* trueExpression;
    Expression* elseExpression;
  };

  Arithmetic* arithmetic;
  Negation* negation;
  Comparison* comparison;
  SetLocal* setLocal;
  SetGlobal* setGlobal;
  Ternary* ternary;
  PrimaryExpression* primary;

  void emitBytecode(CompilerOutput& output);
};

class PrimaryExpression {
 public:
  PrimaryExpression(PrimaryType type) : type(type) {}
  ~PrimaryExpression();

  Value value;
  Expression* group;
  int globalVariableIndex;
  int localVariableIndex;

  PrimaryType type;

  void emitBytecode(CompilerOutput& output);
};

}

#endif