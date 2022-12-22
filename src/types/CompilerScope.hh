#ifndef _MY_MATRIX_TOOLBOX_TYPES_COMPILER_SCOPE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_COMPILER_SCOPE_H_

#include "Object.hh"

namespace mymatrixtoolbox {

struct CompilerScope {
  std::string name;
  Chunk chunk;
  std::vector<Upvalue> upvalues;
};

}

#endif