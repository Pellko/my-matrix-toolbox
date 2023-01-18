#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_H_

#include <string>
#include "Chunk.hh"

namespace mymatrixtoolbox {

enum class ObjectType {
  CLOSURE,
  UPVALUE,
  STRING,
  NATIVE,
  MATRIX,
  CLASS,
  INSTANCE,
  INSTANCE_METHOD,
  MAP,
  MODULE,
};

class Object {
 public:
  Object(ObjectType type) : type(type) {}
  virtual ~Object() {}

  virtual std::string toString() = 0;

  ObjectType type;
  bool isMarked = false;
};

}

#endif