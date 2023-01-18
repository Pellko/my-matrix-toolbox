#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_MODULE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_MODULE_H_

#include "Object.hh"
#include "ObjectClosure.hh"
#include "Value.hh"

namespace mymatrixtoolbox {

class ObjectModule : public Object {
 public:
  ObjectModule() : Object(ObjectType::MODULE) {}
  ~ObjectModule() {}

  int moduleIndex;
  std::vector<Value> globals;
  std::unordered_map<std::string, int> globalNames;
  ObjectClosure* closure;

  std::string toString() override {
    return "<module>";
  }
};

}

#endif