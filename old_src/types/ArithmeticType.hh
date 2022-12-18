#ifndef _SCISCRIPT_TYPES_ARITHMETIC_TYPE_H_
#define _SCISCRIPT_TYPES_ARITHMETIC_TYPE_H_

namespace sciscript {

enum class ArithmeticType {
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
};

enum class ComparisonType {
  EQUALS,
  LT,
  LEQ,
  GT,
  GEQ,
};

}

#endif