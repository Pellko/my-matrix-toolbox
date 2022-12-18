#ifndef _SCISCRIPT_TYPES_UPVALUE_H_
#define _SCISCRIPT_TYPES_UPVALUE_H_

namespace sciscript {

struct Upvalue {
  bool isLocal;
  int index;
};

}

#endif