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
};

class Object {
 public:
  Object(ObjectType type) : type(type) {}
  virtual ~Object() {}

  ObjectType type;
  bool isMarked = false;
};

}

#endif