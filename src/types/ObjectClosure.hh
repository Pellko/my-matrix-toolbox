#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_UPVALUE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_UPVALUE_H_

#include "Object.hh"
#include "Value.hh"

namespace mymatrixtoolbox {

class ObjectUpvalue : public Object {
 public:
  ObjectUpvalue() : Object(ObjectType::UPVALUE) {}
  ~ObjectUpvalue() {}

  int stackIndex;
  Value closed;
  bool isClosed;
  ObjectUpvalue* next;
};

class ObjectClosure : public Object {
 public:
  ObjectClosure() : Object(ObjectType::CLOSURE) {}
  ~ObjectClosure() {}

  std::string name;
  int functionIndex;
  std::vector<ObjectUpvalue*> upvalues;
};


}

#endif