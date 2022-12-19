#ifndef _SCISCRIPT_COMPILER_DISASSEMBLER_H_
#define _SCISCRIPT_COMPILER_DISASSEMBLER_H_

#include "src/types/Chunk.hh"
#include "src/types/Literal.hh"
#include "src/types/CompilerOutput.hh"

#include <string>

namespace sciscript {

class Disassembler {
 public:
  Disassembler() {}
  ~Disassembler() {}

  void disassemble(CompilerOutput& output);

 private:
  void disassembleChunk(Chunk& chunk, CompilerOutput& output);
  int simpleInstruction(std::string instruction, int offset);
  int constantInstruction(std::string instruction, int offset, Chunk& chunk);
  int readVariable(std::string instruction, int offset, Chunk& chunk);
  int setVariable(std::string instruction, int offset, Chunk& chunk);
  int callInstruction(std::string instruction, int offset, Chunk& chunk);
  int closureInstruction(std::string instruction, int offset, Chunk& chunk, CompilerOutput& output);
  int jumpInstruction(std::string instruction, int offset, Chunk& chunk);

  std::pair<int, int> readDynamicBytes(int offset, Chunk& chunk);
  std::string printLiteral(Literal literal);
};

}

#endif