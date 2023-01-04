#ifndef _MY_MATRIX_TOOLBOX_TYPES_OBJECT_MAP_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OBJECT_MAP_H_

#include "Object.hh"
#include "Value.hh"
#include <unordered_map>
#include <sstream>

namespace mymatrixtoolbox {

class ObjectMap : public Object {
 public:
  ObjectMap() : Object(ObjectType::MAP) {}
  ~ObjectMap() {}
  std::unordered_map<std::string, Value> values;

  std::string toString() override {
    std::stringstream ss;
    ss << "{" << std::endl;

    for(auto value : values) {
      ss << "  " << value.first << ": " << value.second.toString() << "," << std::endl;
    }

    ss << std::endl << "}" << std::endl;

    return ss.str();
  }
};
  
}

#endif
