#ifndef _MY_MATRIX_TOOLBOX_TYPES_CHUNK_H_
#define _MY_MATRIX_TOOLBOX_TYPES_CHUNK_H_

#include <vector>
#include <unordered_map>
#include "Literal.hh"
#include "Upvalue.hh"

namespace mymatrixtoolbox {

struct Chunk {
  std::vector<uint8_t> bytecode;
  std::vector<Literal> literals;
  int numUpvalues = 0;
  int numArguments = 0;

  void free() {
    for(auto& literal : literals) {
      literal.free();
    }
  }

  void emitByte(uint8_t);
  void emitDynamicBytes(int v);
  int registerStringLiteral(std::string name);
};

namespace code {

std::vector<uint8_t> generateDynamicBytes(int value);

}

}

#endif
