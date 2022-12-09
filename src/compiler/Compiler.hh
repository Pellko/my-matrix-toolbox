#ifndef _SCISCRIPT_COMPILER_COMPILER_H_
#define _SCISCRIPT_COMPILER_COMPILER_H_

#include <vector>
#include <variant>
#include <ctype.h>
#include "Lexer.hh"
#include "src/compiler/Expression.hh"
#include "src/vm/VirtualMachine.hh"
#include "SyntaxException.hh"

namespace sciscript {

struct CompilerOutput {
  std::vector<uint8_t> bytecode;
  std::vector<Value> constants;

  void pushConstant(Value value) {
    constants.push_back(value);
    emitConstant(constants.size()-1);
  }

  void emitConstant(int index) {
    int i = index;
    unsigned char bytes = 1;
    unsigned char max = 128;

    while(max <= i) {
      i >>= 8;
      bytes++;
    }

    emitByte(OP_CONSTANT);
    emitByte(bytes);
    for(int i=0;i<bytes;i++) {
      emitByte((index >> i * 8) & 0xFF);
    }
  }

  void emitByte(uint8_t b) {
    bytecode.push_back(b);
  }
};

class Compiler {
 public:
  Compiler(std::vector<Token>& tokens);
  ~Compiler() {}

  CompilerOutput compile();

 private:
  std::vector<Token>& tokens;
  int position;

  Token* peek(int n=0);
  Token* get();
  bool empty();
  
  void expression(CompilerOutput& output);
  Expression* readExpression(bool fromGroup = false);
  Expression* readTerm();
  std::variant<Expression*, PrimaryExpression*> readFactor();
  PrimaryExpression* readPrimary();
};

}

#endif