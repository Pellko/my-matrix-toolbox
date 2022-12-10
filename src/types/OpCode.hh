#ifndef _SCISCRIPT_TYPES_OP_CODE_H_
#define _SCISCRIPT_TYPES_OP_CODE_H_

namespace sciscript {

enum OpCode {
  OP_RETURN,
  OP_ADD,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_MOD,
  OP_NEG,
  OP_CONSTANT,
  OP_SET_GLOBAL,
  OP_READ_GLOBAL,
  OP_SET_LOCAL,
  OP_READ_LOCAL,
  OP_PRINT,
  OP_POP,
};

}

#endif