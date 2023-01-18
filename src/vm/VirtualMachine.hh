#ifndef _MY_MATRIX_TOOLBOX_VM_VIRTUAL_MACHINE_H_
#define _MY_MATRIX_TOOLBOX_VM_VIRTUAL_MACHINE_H_

#include "src/compiler/ast/BinaryExpression.hh"
#include "src/types/Chunk.hh"
#include "src/types/CompilerOutput.hh"
#include "src/types/ObjectClosure.hh"
#include "src/types/ObjectString.hh"
#include "src/types/ObjectNative.hh"
#include "src/types/ObjectModule.hh"
#include "src/types/Value.hh"
#include "CallFrame.hh"

namespace mymatrixtoolbox {

class VirtualMachine {
 public:
  VirtualMachine(ExecutionContext* context) : context(context) {}
  ~VirtualMachine() {}

  void initialize(CompilerOutput& output);
  void execute(CompilerOutput& output);
  void registerNativeFunction(int index, NativeFunction function);
  void registerNativeObjectMethod(ObjectType type, std::string name, NativeFunction function);

  Object* allocateObject(ObjectType type);
 private:
  ExecutionContext* context;
  std::vector<Value> globals;
  std::vector<Value> valueStack;
  std::vector<CallFrame> callFrames;
  std::vector<ObjectModule*> modules;
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

  std::vector<Value>& getGlobalsList();
  Chunk* getChunk(CompilerOutput& output, int moduleId, int functionIndex);

  bool debugGarbageCollector = false;
};

}

#endif