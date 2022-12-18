#ifndef _SCISCRIPT_TYPES_OBJECT_H_
#define _SCISCRIPT_TYPES_OBJECT_H_

#include <string>
#include "Chunk.hh"

namespace sciscript {

enum class ObjectType {
  FUNCTION,
  CLOSURE,
  UPVALUE,
};

class Object {
 public:
  Object(ObjectType type) : type(type) {}
  virtual ~Object() {}

  ObjectType type;
};

}

#endif