#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include "src/compiler/Compiler.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/vm/RuntimeException.hh"
#include "src/vm/VirtualMachine.hh"

using namespace sciscript;

std::string read_string_from_file(const std::string &file_path) {
  const std::ifstream input_stream(file_path, std::ios_base::binary);

  if (input_stream.fail()) {
    throw std::runtime_error("Failed to open file");
  }

  std::stringstream buffer;
  buffer << input_stream.rdbuf();

  return buffer.str();
}

int main(int argc, char** argv) {
  std::string code = read_string_from_file("./examples/fibbonaci.txt");

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