#ifndef _MY_MATRIX_TOOLBOX_TYPES_COMPILER_OUTPUT_H_
#define _MY_MATRIX_TOOLBOX_TYPES_COMPILER_OUTPUT_H_

#include "CompilerScope.hh"
#include <vector>

namespace mymatrixtoolbox {

struct CompilerOutput {
  Chunk root;
  std::vector<Chunk> functions;
  int numGlobals;

  void free() {
    root.free();
    for(Chunk& chunk : functions) {
      chunk.free();
    }
  }
};

}

#endif