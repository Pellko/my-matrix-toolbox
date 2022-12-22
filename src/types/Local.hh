#ifndef _MY_MATRIX_TOOLBOX_TYPES_LOCAL_H_
#define _MY_MATRIX_TOOLBOX_TYPES_LOCAL_H_

#include <string>

namespace mymatrixtoolbox {

enum class VarRefType {
  LOCAL,
  GLOBAL,
  UPVALUE,
};

struct Local {
  std::string name;
  int depth;
  bool hasClosureReference;
};

}

#endif