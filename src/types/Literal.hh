#ifndef _MY_MATRIX_TOOLBOX_TYPES_LITERAL_H_
#define _MY_MATRIX_TOOLBOX_TYPES_LITERAL_H_

#include <string>

namespace mymatrixtoolbox {

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

  static Literal fromString(std::string string) {
    Literal l;
    l.type = LiteralType::STRING;
    l.as.str = new std::string(string);
    return l;
  }
};

}

#endif