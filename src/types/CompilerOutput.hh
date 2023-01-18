#ifndef _MY_MATRIX_TOOLBOX_TYPES_COMPILER_OUTPUT_H_
#define _MY_MATRIX_TOOLBOX_TYPES_COMPILER_OUTPUT_H_

#include "CompilerScope.hh"
#include <vector>

namespace mymatrixtoolbox {

struct ModuleOutput {
  Chunk root;
  std::vector<Chunk> functions;
  std::vector<std::string> globalNames;

  void free() {
    root.free();
    for(Chunk& chunk : functions) {
      chunk.free();
    }
  }
};

struct CompilerOutput {
  Chunk root;
  std::vector<Chunk> functions;
  std::vector<ModuleOutput> modules;
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