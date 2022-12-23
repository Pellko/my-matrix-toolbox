#ifndef _MY_MATRIX_TOOLBOX_STDLIB_LIBRARY_H_
#define _MY_MATRIX_TOOLBOX_STDLIB_LIBRARY_H_

#include "src/types/ObjectNative.hh"
#include "src/compiler/Compiler.hh"
#include "src/vm/VirtualMachine.hh"

namespace mymatrixtoolbox {

class Library {
 public:
  Library() {}
  ~Library() {}

  void registerFunction(std::string name, NativeFunction function) {
    functions.push_back(FunctionDefinition{
      .name = name,
      .fn = function,
      .index = -1
    });
  }

  void registerObjectMethod(std::string name, ObjectType type, NativeFunction function) {
    objectMethods[type][name] = function;
  }

  void registerCompiler(Compiler* compiler) {
    for(auto& def : functions) {
      def.index = compiler->declareNativeFunction(def.name);
    }
  }

  void registerVirtualMachine(VirtualMachine* vm) {
    for(auto& def : functions) {
      vm->registerNativeFunction(def.index, def.fn);
    }
    for(auto& type : objectMethods) {
      for(auto& def : type.second) {
        vm->registerNativeObjectMethod(type.first, def.first, def.second);
      }
    }
  }

  struct FunctionDefinition {
    std::string name;
    NativeFunction fn;
    int index;
  };

 private:
  std::vector<FunctionDefinition> functions;
  std::unordered_map<ObjectType, std::unordered_map<std::string, NativeFunction>> objectMethods;
};

}

#endif