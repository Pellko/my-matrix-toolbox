#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_STRING_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_STRING_H_

#include "Object.hh"

namespace mymatrixtoolbox {

class ObjectString : public Object {
 public:
  ObjectString() : Object(ObjectType::STRING) {}
  ~ObjectString() {}

  void setString(std::string str) {
    this->str = str;
  }

  std::string& getString() {
    return str;
  }

 private:
  std::string str;
};

}

#endif