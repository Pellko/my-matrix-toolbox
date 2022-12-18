#ifndef _SCISCRIPT_VM_CALL_FRAME_H_
#define _SCISCRIPT_VM_CALL_FRAME_H_

#include "src/types/Chunk.hh"
#include "src/types/ObjectClosure.hh"

namespace sciscript {

struct CallFrame {
  int localsOffset;
  int returnAddress;
  Chunk* chunk;
  ObjectClosure* closure;

  void free() {
    chunk->free();
    delete chunk;
  }
};

}

#endif