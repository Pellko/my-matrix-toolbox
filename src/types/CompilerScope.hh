#ifndef _SCISCRIPT_TYPES_COMPILER_SCOPE_H_
#define _SCISCRIPT_TYPES_COMPILER_SCOPE_H_

#include "Object.hh"

namespace sciscript {

struct CompilerScope {
  std::string name;
  Chunk chunk;
  std::vector<Upvalue> upvalues;
};

}

#endif