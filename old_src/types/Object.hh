#ifndef _SCISCRIPT_TYPES_OBJECT_H_
#define _SCISCRIPT_TYPES_OBJECT_H_

#include <string>

namespace sciscript {

enum class ObjectType {
  STRING,
};

class Object {
 public:
  Object(ObjectType type) : type(type) {}
  virtual ~Object() {}

  ObjectType type;
};

class ObjectString : public Object {
 public:
  ObjectString(std::string value) : Object(ObjectType::STRING), value(value) {}
  virtual ~ObjectString() {}

  std::string value;
};

}

#endif