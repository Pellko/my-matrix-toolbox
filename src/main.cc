#include "src/vm/VirtualMachine.hh"
#include "src/vm/RuntimeException.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/Compiler.hh"
#include "src/types/Chunk.hh"
#include "src/types/CompilerOutput.hh"
#include <iostream>
#include <fstream>
#include <sstream>

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

static Value clo(std::vector<Value> args) {
  return Value::fromDouble((double) clock() / CLOCKS_PER_SEC);
}

int main(int argc, char** argv) {
  std::string code = read_string_from_file("./examples/test.txt");
  std::vector<Token> tokens;
  CompilerOutput output;

  try {
    Lexer lexer(code);
    lexer.lex(tokens);

    Compiler compiler(tokens);

    int CLOCK_ID = compiler.declareNativeFunction("clock");

    compiler.compile(output);

    Disassembler disassembler;
    disassembler.disassemble(output);

    std::cout << "=========== EXECUTING ===========" << std::endl;
    VirtualMachine machine;
    machine.initialize(output);
    machine.registerNativeFunction(CLOCK_ID, &clo);
    machine.execute(output);
    std::cout << "=========== FINISHED EXECUTING ===========" << std::endl;
  } catch(SyntaxException* ex) {
    std::cout << "Syntax Exception: " << ex->what() << std::endl;
  } catch(RuntimeException* ex) {
    std::cout << "Runtime Exception: " << ex->what() << std::endl;
  }
}