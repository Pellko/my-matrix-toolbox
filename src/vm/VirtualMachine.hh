#ifndef _SCISCRIPT_VM_VIRTUAL_MACHINE_H_
#define _SCISCRIPT_VM_VIRTUAL_MACHINE_H_

#include "src/compiler/ast/BinaryExpression.hh"
#include "src/types/CompilerOutput.hh"
#include "src/types/ObjectClosure.hh"
#include "src/types/Value.hh"
#include "CallFrame.hh"

namespace sciscript {

class VirtualMachine {
 public:
  VirtualMachine() {}
  ~VirtualMachine() {}

  void execute(CompilerOutput& output);

 private:
  std::vector<Value> globals;
  std::vector<Value> valueStack;
  std::vector<CallFrame> callFrames;
  ObjectUpvalue* openUpvalues;

  std::pair<int, int> readDynamicBytes(std::vector<uint8_t>& bytecode, int position);
  void binaryOp(std::vector<uint8_t>& bytecode, BinaryOperation op);
  void printValue(Value value);
  
  ObjectUpvalue* getUpvalue(int stackIndex);
  void closeUpvalues(int lastIndex);
};

}

#endif