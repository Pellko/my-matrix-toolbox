#include "src/vm/VirtualMachine.hh"
#include "src/vm/RuntimeException.hh"
#include "src/compiler/Disassembler.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/Compiler.hh"
#include "src/types/Chunk.hh"
#include "src/types/CompilerOutput.hh"
#include "src/math/ObjectMatrix.hh"
#include "src/stdlib/MatrixLibrary.hh"
#include "src/graphics/Window.hh"
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

static Value toUppercase(VirtualMachine* vm, std::vector<Value> args) {
  ObjectString* a = static_cast<ObjectString*>(args[0].as.object);
  ObjectString* result = static_cast<ObjectString*>(vm->allocateObject(ObjectType::STRING));
  result->setString(a->getString());
  for(auto& c : result->getString()) {
    c = toupper(c);
  }
  return Value::fromObject(result);
}

int main(int argc, char** argv) {
  // std::string code = read_string_from_file("./examples/test.txt");
  // std::vector<Token> tokens;
  // CompilerOutput output;

  // Library matrixLibrary = stdlib::createMatrixLibrary();

  // try {
  //   Lexer lexer(code);
  //   lexer.lex(tokens);

  //   Compiler compiler(tokens);
  //   matrixLibrary.registerCompiler(&compiler);
  //   compiler.compile(output);

  //   Disassembler disassembler;
  //   disassembler.disassemble(output);

  //   std::cout << "=========== EXECUTING ===========" << std::endl;
  //   VirtualMachine machine;
  //   machine.initialize(output);
  //   matrixLibrary.registerVirtualMachine(&machine);
  //   machine.registerNativeObjectMethod(ObjectType::STRING, "toUppercase", &toUppercase);
  //   machine.execute(output);
  //   std::cout << "=========== FINISHED EXECUTING ===========" << std::endl;
  // } catch(SyntaxException* ex) {
  //   std::cout << "Syntax Exception: " << ex->what() << std::endl;
  // } catch(RuntimeException* ex) {
  //   std::cout << "Runtime Exception: " << ex->what() << std::endl;
  // }

  Window window("My Matrix Toolbox");
  window.init();
  window.run();
  window.terminate();

  return 0;
}