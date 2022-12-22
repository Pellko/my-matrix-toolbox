#ifndef _MY_MATRIX_TOOLBOX_VM_CALL_FRAME_H_
#define _MY_MATRIX_TOOLBOX_VM_CALL_FRAME_H_

#include "src/types/Chunk.hh"
#include "src/types/ObjectClosure.hh"

namespace mymatrixtoolbox {

struct CallFrame {
  int localsOffset;
  int returnAddress;
  Chunk* chunk;
  ObjectClosure* closure;
};

}

#endif