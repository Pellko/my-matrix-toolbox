#ifndef _SCISCRIPT_VM_VIRTUAL_MACHINE_H_
#define _SCISCRIPT_VM_VIRTUAL_MACHINE_H_

#include <vector>
#include <stack>
#include "src/types/Value.hh"
#include "src/types/Object.hh"
#include "src/types/CompilerOutput.hh"
#include "src/types/ArithmeticType.hh"
#include "RuntimeException.hh"

namespace sciscript {

class VirtualMachine {
 public:
  VirtualMachine() {}
  ~VirtualMachine() {}

  void execute(CompilerOutput& code);
 
 private:
  std::vector<Value> globals;
  std::vector<Value> valueStack;
  std::pair<int, int> readDynamicBytes(std::vector<uint8_t>& bytecode, int position);
  void binaryOp(std::vector<uint8_t>& bytecode, ArithmeticType type);
};

}

#endif
