#ifndef _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_
#define _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_

#include "CompilerScope.hh"
#include <vector>

namespace sciscript {

struct CompilerOutput {
  Chunk root;
  std::vector<Chunk> functions;
  int numGlobals;
};

}

#endif