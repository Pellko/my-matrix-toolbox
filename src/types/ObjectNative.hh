#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_NATIVE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_NATIVE_H_

#include "Object.hh"
#include "Value.hh"
#include <vector>

namespace mymatrixtoolbox {

typedef Value (*NativeFunction)(std::vector<Value> args);

class ObjectNative : public Object {
 public:
  ObjectNative() : Object(ObjectType::NATIVE) {}
  ~ObjectNative() {}

  NativeFunction function;
  Object* owner = nullptr;
};

}

#endif
