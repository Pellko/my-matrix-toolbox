#ifndef _SCISCRIPT_TYPES_VALUE_H_
#define _SCISCRIPT_TYPES_VALUE_H_

#include "Object.hh"
#include "Literal.hh"
#include <memory>

namespace sciscript {

enum class ValueType {
  BOOL,
  NIL,
  NUMBER,
  OBJECT,
};

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
    Object* object;
  } as;
  
  static Value fromDouble(double d) {
    return Value{
      .type = ValueType::NUMBER,
      .as.number = d,
    };
  }

  static Value fromBool(bool b) {
    return Value{
      .type = ValueType::BOOL,
      .as.boolean = b,
    };
  }

  static Value nil() {
    return Value{
      .type = ValueType::NIL,
    };
  }

  static Value fromObject(Object* object) {
    return Value{
      .type = ValueType::OBJECT,
      .as.object = object,
    };
  }

  static Value fromLiteral(Literal literal) {
    switch(literal.type) {
      case LiteralType::NUMBER:
        return Value::fromDouble(literal.as.number);
      case LiteralType::BOOL:
        return Value::fromBool(literal.as.boolean);
      case LiteralType::NIL:
        return Value::nil();
      case LiteralType::STRING:
        return Value::nil();
    }
  }
};

}

#endif