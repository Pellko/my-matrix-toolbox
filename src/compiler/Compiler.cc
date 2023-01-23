#include "Compiler.hh"
#include "src/context/Runfiles.hh"
#include "src/types/CompilerScope.hh"
#include "src/types/Object.hh"
#include "src/types/OpCode.hh"
#include <fstream>

namespace mymatrixtoolbox {

void Compiler::compile(CompilerOutput& compilerOutput) {
  // Compile main
  rootScope = new CompilerScope;
  rootScope->name = "main";

  while(!parserTool.empty()) {
    statement();
  }
  rootScope->chunk.emitByte(OP_RETURN);

  // Compile modules
  for(std::string& name : parserTool.getImports()) {
    compileModule(name, compilerOutput);
  }

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

void Compiler::compileModule(std::string name, CompilerOutput& compilerOutput) {
  std::string contents = readFile("examples/" + name + ".txt");
  std::vector<Token> tokens;
  Lexer lexer(contents);
  lexer.lex(tokens);
  ParserTool moduleTool(tokens);

  CompilerScope* moduleScope = new CompilerScope;
  moduleScope->name = "<module_" + name + ">";

  while(!moduleTool.empty()) {
    std::shared_ptr<Statement> statement = Statement::parse(moduleTool);
    statement->emitBytecode(moduleScope->chunk);
  }

  moduleScope->chunk.emitByte(OP_RETURN);

  ModuleOutput moduleOutput = {};
  moduleOutput.root = moduleScope->chunk;
  for(std::string name : moduleTool.getGlobals()) {
    moduleOutput.globalNames.push_back(name);
  }

  for(CompilerScope* scope : moduleTool.getFunctions()) {
    moduleOutput.functions.push_back(scope->chunk);
  }

  compilerOutput.modules.push_back(moduleOutput);

  delete moduleScope;
  for(CompilerScope* scope : moduleTool.getFunctions()) {
    delete scope;
  }
}

void Compiler::statement() {
  std::shared_ptr<Statement> statement = Statement::parse(parserTool);
  statement->emitBytecode(rootScope->chunk);
}

std::string Compiler::readFile(std::string fileName) {
  const std::ifstream inputStream(Runfiles::convert(fileName), std::ios_base::binary);

  if (inputStream.fail()) {
    throw std::runtime_error("Failed to open file");
  }

  std::stringstream buffer;
  buffer << inputStream.rdbuf();

  return buffer.str();
}

int Compiler::declareNativeFunction(std::string name) {
  return parserTool.registerGlobal(name);
}

}