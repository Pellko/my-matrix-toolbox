#ifndef _MY_MATRIX_TOOLBOX_VM_VIRTUAL_MACHINE_H_
#define _MY_MATRIX_TOOLBOX_VM_VIRTUAL_MACHINE_H_

#include "src/compiler/ast/BinaryExpression.hh"
#include "src/types/CompilerOutput.hh"
#include "src/types/ObjectClosure.hh"
#include "src/types/ObjectString.hh"
#include "src/types/ObjectNative.hh"
#include "src/types/Value.hh"
#include "CallFrame.hh"

namespace mymatrixtoolbox {

class VirtualMachine {
 public:
  VirtualMachine() {}
  ~VirtualMachine() {}

  void initialize(CompilerOutput& output);
  void execute(CompilerOutput& output);
  void registerNativeFunction(int index, NativeFunction function);
  void registerNativeObjectMethod(ObjectType type, std::string name, NativeFunction function);

  Object* allocateObject(ObjectType type);
 private:
  std::vector<Value> globals;
  std::vector<Value> valueStack;
  std::vector<CallFrame> callFrames;
  std::vector<Object*> objects;
  std::unordered_map<ObjectType, std::unordered_map<std::string, NativeFunction>> nativeObjectMethods;
  ObjectUpvalue* openUpvalues;

  std::pair<int, int> readDynamicBytes(std::vector<uint8_t>& bytecode, int position);
  void numericOp(std::vector<uint8_t>& bytecode, BinaryOperation op);
  void comparisonOp(std::vector<uint8_t>& bytecode, BinaryOperation op);
  void printValue(Value value);

  ObjectUpvalue* getUpvalue(int stackIndex);
  void closeUpvalues(int lastIndex);

  std::vector<Object*> gcWorkingStack;
  void collectGarbage();
  void markRoots();
  void traceReferences();
  void sweep();

  void traverseObjectReferences(Object* object);
  void markValue(Value value);
  void markObject(Object* object);

  bool debugGarbageCollector = false;
};

}

#endif