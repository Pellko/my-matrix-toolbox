#include "src/vm/VirtualMachine.hh"
#include "src/vm/RuntimeException.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/Compiler.hh"
#include "src/types/Chunk.hh"
#include "src/types/CompilerOutput.hh"
#include "src/math/ObjectMatrix.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace mymatrixtoolbox;

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

static Value solve(std::vector<Value> args) {
  ObjectMatrix* a = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* b = static_cast<ObjectMatrix*>(args[1].as.object);
  a->rowEchelon(*b);
  return Value::nil();
}

static Value toUppercase(std::vector<Value> args) {
  ObjectString* a = static_cast<ObjectString*>(args[0].as.object);
  for(auto& c : a->getString()) {
    c = toupper(c);
  }
  return Value::nil();
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
    machine.registerNativeObjectMethod(ObjectType::MATRIX, "solve", &solve);
    machine.registerNativeObjectMethod(ObjectType::STRING, "toUppercase", &toUppercase);
    machine.execute(output);
    std::cout << "=========== FINISHED EXECUTING ===========" << std::endl;
  } catch(SyntaxException* ex) {
    std::cout << "Syntax Exception: " << ex->what() << std::endl;
  } catch(RuntimeException* ex) {
    std::cout << "Runtime Exception: " << ex->what() << std::endl;
  }
}