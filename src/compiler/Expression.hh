#ifndef _SCISCRIPT_COMPILER_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_EXPRESSION_H_

#include "src/vm/VirtualMachine.hh"
#include <iostream>

namespace sciscript {

struct CompilerOutput;
class PrimaryExpression;

enum class ExpressionType {
  ARITHMETIC,
  NEGATION,
  PRIMARY,
};

enum class ArithmeticType {
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
};

enum class PrimaryType {
  CONSTANT,
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

  Arithmetic* arithmetic;
  Negation* negation;
  PrimaryExpression* primary;

  void emitBytecode(CompilerOutput& output);
};

class PrimaryExpression {
 public:
  PrimaryExpression(PrimaryType type) : type(type) {}
  ~PrimaryExpression();

  Value value;
  Expression* group;

  PrimaryType type;

  void emitBytecode(CompilerOutput& output);
};

}

#endif