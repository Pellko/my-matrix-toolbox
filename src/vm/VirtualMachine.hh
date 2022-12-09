#ifndef _SCISCRIPT_VM_VIRTUAL_MACHINE_H_
#define _SCISCRIPT_VM_VIRTUAL_MACHINE_H_

#include <vector>

namespace sciscript {

enum OpCode {
  OP_RETURN,
  OP_ADD,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
  OP_MOD,
  OP_NEG,
  OP_CONSTANT
};

typedef double Value;

class VirtualMachine {
 public:
  VirtualMachine() {}
  ~VirtualMachine() {}

  void execute(std::vector<uint8_t> bytecode);
};

}

#endif
