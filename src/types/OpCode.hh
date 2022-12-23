#ifndef _MY_MATRIX_TOOLBOX_TYPES_OP_CODE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_OP_CODE_H_

namespace mymatrixtoolbox {

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
  OP_EQUALS,
  OP_JUMP_FALSE,
  OP_JUMP,
  OP_LOOP,
  OP_LT,
  OP_LEQ,
  OP_CALL,
  OP_CLOSURE,
  OP_READ_UPVALUE,
  OP_SET_UPVALUE,
  OP_CLOSE_UPVALUE,
  OP_MATRIX,
  OP_MATRIX_ACCESS,
  OP_CLASS,
  OP_METHOD,
  OP_SET_PROPERTY,
  OP_READ_PROPERTY,
};

}

#endif