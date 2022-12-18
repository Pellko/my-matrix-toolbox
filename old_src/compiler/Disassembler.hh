#ifndef _SCISCRIPT_COMPILER_DISASSEMBLER_H_
#define _SCISCRIPT_COMPILER_DISASSEMBLER_H_

#include <string>
#include <sstream>
#include "Compiler.hh"

namespace sciscript {

class Disassembler {
 public:
  Disassembler() {}
  ~Disassembler() {}

  void disassemble(CompilerOutput& code, std::string& output);

 private:
  int simpleInstruction(std::string instruction, int offset, std::stringstream& ss);
  int constantInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss);
  int globalInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss);
  int localInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss);
  int jumpInstruction(std::string instruction, int offset, CompilerOutput& code, std::stringstream& ss);
  std::string printValue(Value value);
  std::string printObject(Object* object);
};

}

#endif
