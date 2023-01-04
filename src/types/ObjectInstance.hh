#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_INSTANCE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_INSTANCE_H_

#include "Object.hh"
#include "ObjectClass.hh"
#include <unordered_map>

namespace mymatrixtoolbox {

class ObjectInstance : public Object {
 public:
  ObjectInstance() : Object(ObjectType::INSTANCE) {}
  ~ObjectInstance() {}

  ObjectClass* classTemplate;
  std::unordered_map<std::string, Value> fields;

  std::string toString() override {
    return "<instance>";
  }
};

}

#endif