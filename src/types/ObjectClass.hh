#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_CLASS_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_CLASS_H_

#include "Object.hh"
#include "ObjectClosure.hh"
#include <unordered_map>

namespace mymatrixtoolbox {

class ObjectClass : public Object {
 public:
  ObjectClass() : Object(ObjectType::CLASS) {}
  ~ObjectClass() {}

  std::unordered_map<std::string, ObjectClosure*> methods;
};

}

#endif
