#ifndef _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_
#define _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_

#include <vector>
#include <unordered_map>
#include "Value.hh"
#include "OpCode.hh"

namespace sciscript {

struct CompilerOutput {
  std::vector<uint8_t> bytecode;
  std::vector<Value> constants;
  std::vector<std::string> globals;
  std::unordered_map<std::string, int> globalNames;

  void pushConstant(Value value) {
    constants.push_back(value);
    emitConstant(constants.size()-1);
  }

  void pushGlobal(std::string name) {
    globals.push_back(name);
    globalNames[name] = globals.size() - 1;
    emitByte(OP_DEFINE_GLOBAL);
    emitDynamicBytes(globals.size() - 1);
  }

  void emitConstant(int index) {
    emitByte(OP_CONSTANT);
    emitDynamicBytes(index);
  }

  void emitDynamicBytes(int value) {
    int i = value;
    unsigned char bytes = 1;
    unsigned char max = 128;

    while(max <= i) {
      i >>= 8;
      bytes++;
    }

    emitByte(bytes);
    for(int i=0;i<bytes;i++) {
      emitByte((value >> i * 8) & 0xFF);
    }
  }

  void emitByte(uint8_t b) {
    bytecode.push_back(b);
  }

  void free() {
    for(Value& value : constants) {
      if(IS_OBJECT(value)) {
        Object* obj = AS_OBJECT(value);
        delete obj;
      }
    }
  }
};

}

#endif