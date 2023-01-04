#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_INSTANCE_METHOD_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_INSTANCE_METHOD_H_

#include "Object.hh"
#include "ObjectInstance.hh"

namespace mymatrixtoolbox {

class ObjectInstanceMethod : public Object {
 public:
  ObjectInstanceMethod() : Object(ObjectType::INSTANCE_METHOD) {}
  ~ObjectInstanceMethod() {}

  ObjectInstance* instance;
  ObjectClosure* method;

  std::string toString() override {
    return "<method>";
  }
};

}

#endif
