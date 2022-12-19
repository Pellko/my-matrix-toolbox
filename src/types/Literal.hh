#ifndef _SCISCRIPT_TYPES_LITERAL_H_
#define _SCISCRIPT_TYPES_LITERAL_H_

#include <string>

namespace sciscript {

enum class LiteralType {
  NUMBER,
  BOOL,
  NIL,
  STRING,
};

struct Literal {
  LiteralType type;
  union {
    bool boolean;
    double number;
    std::string* str;
  } as;

  void free() {
    if(type == LiteralType::STRING) {
      delete as.str;
    }
  }

  static Literal fromDouble(double d) {
    Literal l;
    l.type = LiteralType::NUMBER;
    l.as.number = d;
    return l;
  }

  static Literal nil() {
    return Literal{
      .type = LiteralType::NIL,
    };
  }

  static Literal fromBool(bool b) {
    Literal l;
    l.type = LiteralType::BOOL;
    l.as.boolean = b;
    return l;
  }
};

}

#endif