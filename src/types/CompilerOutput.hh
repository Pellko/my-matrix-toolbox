#ifndef _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_
#define _SCISCRIPT_TYPES_COMPILER_OUTPUT_H_

#include <vector>
#include <unordered_map>
#include "Value.hh"
#include "OpCode.hh"
#include "Local.hh"

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
    emitByte(OP_SET_GLOBAL);
    emitDynamicBytes(globals.size() - 1);
  }

  void setGlobal(std::string name) {
    int index = globalNames[name];
    emitByte(OP_SET_GLOBAL);
    emitDynamicBytes(index);
  }

  void setLocal(std::string name, int index) {
    emitByte(OP_SET_LOCAL);
    emitDynamicBytes(index);
  }

  void readGlobal(int index) {
    emitByte(OP_READ_GLOBAL);
    emitDynamicBytes(index);
  }

  void readLocal(int index) {
    emitByte(OP_READ_LOCAL);
    emitDynamicBytes(index);
  }

  void emitConstant(int index) {
    emitByte(OP_CONSTANT);
    emitDynamicBytes(index);
  }

  void emitDynamicBytes(int value) {
    std::vector<uint8_t> result = generateDynamicBytes(value);
    for(int i=0;i<result.size();i++) {
      emitByte(result[i]);
    }
  }

  std::vector<uint8_t> generateDynamicBytes(int value) {
    int i = value;
    unsigned char bytes = 1;
    unsigned char max = 128;

    while(max <= i) {
      i >>= 8;
      bytes++;
    }
    
    std::vector<uint8_t> result;
    result.push_back(bytes);
    for(int i=0;i<bytes;i++) {
      result.push_back((value >> i * 8) & 0xFF);
    }
    
    return result;
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