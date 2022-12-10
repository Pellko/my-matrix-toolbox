#include <exception>
#include <iostream>
#include <vector>
#include "src/compiler/Compiler.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/vm/RuntimeException.hh"
#include "src/vm/VirtualMachine.hh"

using namespace sciscript;

int main(int argc, char** argv) {
  // std::string code = "let test = 100; { let test=200; { let test=10; test=30 ;print(test+20); } }";
  std::string code = "let test = 100; print(test=50);";

  // Perform lexical analysis
  Lexer lexer(code);
  std::vector<Token> tokens;
  lexer.lex(tokens);

  // Compile
  Compiler compiler(tokens);
  Disassembler disassembler;

  try {
    CompilerOutput& compilerOutput = compiler.compile();
    
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
  } catch(RuntimeException* ex) {
    std::cout << ex->what() << std::endl;
  }
  
  return 0;
}