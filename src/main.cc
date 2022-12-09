#include <iostream>
#include <vector>
#include "src/compiler/Compiler.hh"
#include "src/compiler/Disassembler.hh"

using namespace sciscript;

int main(int argc, char** argv) {
  std::string code = "(10 + 2) * 4 / 2";

  // Perform lexical analysis
  Lexer lexer(code);
  std::vector<Token> tokens;
  lexer.lex(tokens);

  // Compile
  Compiler compiler(tokens);
  Disassembler disassembler;
  CompilerOutput compilerOutput = compiler.compile();
  
  // Disassemble
  std::string disassembly;
  disassembler.disassemble(compilerOutput, disassembly);

  return 0;
}