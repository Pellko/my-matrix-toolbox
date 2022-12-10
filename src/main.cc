#include <exception>
#include <iostream>
#include <vector>
#include "src/compiler/Compiler.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/vm/VirtualMachine.hh"

using namespace sciscript;

int main(int argc, char** argv) {
  // std::string code = "let test = 10+20+30;";
  std::string code = "print(10 + 20 + 30);";

  // Perform lexical analysis
  Lexer lexer(code);
  std::vector<Token> tokens;
  lexer.lex(tokens);

  // Compile
  Compiler compiler(tokens);
  Disassembler disassembler;

  try {
    CompilerOutput compilerOutput = compiler.compile();

    // Disassemble
    std::string disassembly;
    disassembler.disassemble(compilerOutput, disassembly);

    std::cout << "=========== EXECUTING ===========" << std::endl;

    // Run
    VirtualMachine vm;
    vm.execute(compilerOutput);

    compilerOutput.free();
  } catch(SyntaxException* ex) {
    std::cout << ex->what() << std::endl;
  }
  
  return 0;
}