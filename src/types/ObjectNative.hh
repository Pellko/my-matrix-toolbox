#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_NATIVE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_NATIVE_H_

#include "Object.hh"
#include "Value.hh"
#include <vector>

namespace mymatrixtoolbox {

class VirtualMachine;
class ExecutionContext;

typedef Value (*NativeFunction)(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args);

class ObjectNative : public Object {
 public:
  ObjectNative() : Object(ObjectType::NATIVE) {}
  ~ObjectNative() {}

  NativeFunction function;
  Object* owner = nullptr;

  std::string toString() override {
    return "<native function>";
  }
};

}

#endif
