#ifndef _MY_MATRIX_TOOLBOX_TYPES_VALUE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_VALUE_H_

#include "Object.hh"
#include "ObjectString.hh"
#include "Literal.hh"
#include <memory>

namespace mymatrixtoolbox {

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

  static Value fromLiteral(Literal literal, std::function<Object* (ObjectType)> allocateObject) {
    switch(literal.type) {
      case LiteralType::NUMBER:
        return Value::fromDouble(literal.as.number);
      case LiteralType::BOOL:
        return Value::fromBool(literal.as.boolean);
      case LiteralType::NIL:
        return Value::nil();
      case LiteralType::STRING:
        ObjectString* obj = static_cast<ObjectString*>(allocateObject(ObjectType::STRING));
        obj->setString(*literal.as.str);
        return Value::fromObject(obj);
    }
  }

  std::string toString() {
    switch(type) {
      case ValueType::NUMBER:
        return std::to_string(as.number);
      case ValueType::BOOL:
        return as.boolean ? "true" : "false";
      case ValueType::NIL:
        return "nil";
      case ValueType::OBJECT:
        return as.object->toString();
      default:
        return "<unexpected type>";
  }
  }
};

}

#endif