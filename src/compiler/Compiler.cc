#include "Compiler.hh"
#include "src/types/CompilerScope.hh"
#include "src/types/Object.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void Compiler::compile(CompilerOutput& compilerOutput) {
  rootScope = new CompilerScope;
  rootScope->name = "main";

  while(!parserTool.empty()) {
    statement();
  }

  rootScope->chunk.emitByte(OP_RETURN);

  compilerOutput.numGlobals = parserTool.getGlobals().size();
  compilerOutput.root = rootScope->chunk;
  for(CompilerScope* scope : parserTool.getFunctions()) {
    compilerOutput.functions.push_back(scope->chunk);
  }

  delete rootScope;
  for(CompilerScope* scope : parserTool.getFunctions()) {
    delete scope;
  }
}

void Compiler::statement() {
  Statement* statement = Statement::parse(parserTool);
  statement->emitBytecode(rootScope->chunk);
  delete statement;
}

int Compiler::declareNativeFunction(std::string name) {
  return parserTool.registerGlobal(name);
}

}