#ifndef _SCISCRIPT_TYPES_OBJECT_NATIVE_H_
#define _SCISCRIPT_TYPES_OBJECT_NATIVE_H_

#include "Object.hh"
#include "Value.hh"
#include <vector>

namespace sciscript {

typedef Value (*NativeFunction)(std::vector<Value> args);

class ObjectNative : public Object {
 public:
  ObjectNative() : Object(ObjectType::NATIVE) {}
  ~ObjectNative() {}

  NativeFunction function;
};

}

#endif
