#ifndef _SCISCRIPT_TYPES_LOCAL_H_
#define _SCISCRIPT_TYPES_LOCAL_H_

#include <string>

namespace sciscript {

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