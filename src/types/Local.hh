#ifndef _SCISCRIPT_TYPES_LOCAL_H_
#define _SCISCRIPT_TYPES_LOCAL_H_

#include <string>

namespace sciscript {

struct Local {
  std::string name;
  int depth;
};

}

#endif