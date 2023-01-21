#include <math.h>
#include "src/context/ExecutionContext.hh"
#include "src/compiler/Compiler.hh"
#include "src/compiler/Disassembler.hh"
#include "src/stdlib/MatrixLibrary.hh"
#include "src/stdlib/PlotLibrary.hh"
#include "src/vm/RuntimeException.hh"

using namespace mymatrixtoolbox;

static Value sin(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args) {
  double input = args[0].as.number;
  return Value::fromDouble(sin(input));
}

int main(int argc, char** argv) {
  std::shared_ptr<ExecutionContext> executionContext = std::make_shared<ExecutionContext>();
  
  executionContext->launch([&]() {
    std::string code = Compiler::readFile("examples/test.txt");
    std::vector<Token> tokens;
    CompilerOutput output;

    Library matrixLibrary = stdlib::createMatrixLibrary();
    Library plotLibrary = stdlib::createPlotLibrary();

    try {
      Lexer lexer(code);
      lexer.lex(tokens);

      Compiler compiler(tokens);
      int SIN_ID = compiler.declareNativeFunction("sin");
      matrixLibrary.registerCompiler(&compiler);
      plotLibrary.registerCompiler(&compiler);
      compiler.compile(output);

      Disassembler disassembler;
      disassembler.disassemble(output);

      std::cout << "=========== EXECUTING ===========" << std::endl;
      VirtualMachine machine(executionContext.get());
      machine.initialize(output);
      machine.registerNativeFunction(SIN_ID, &sin);
      matrixLibrary.registerVirtualMachine(&machine);
      plotLibrary.registerVirtualMachine(&machine);
      machine.execute(output);
      std::cout << "=========== FINISHED EXECUTING ===========" << std::endl;
    } catch(SyntaxException* ex) {
      std::cout << "Syntax Exception: " << ex->what() << std::endl;
    } catch(RuntimeException* ex) {
      std::cout << "Runtime Exception: " << ex->what() << std::endl;
    }
  });

  return 0;
}