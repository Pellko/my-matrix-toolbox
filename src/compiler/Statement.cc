#include "Statement.hh"

namespace sciscript {

Statement::~Statement() {
  if(declareGlobal != nullptr) {
    delete declareGlobal->initializer;
    delete declareGlobal;
  }
  if(print != nullptr) {
    delete print->expression;
    delete print;
  }
}

void Statement::emitBytecode(CompilerOutput& output) {
  switch(type) {
    case StatementType::DECLARE_GLOBAL: {
      declareGlobal->initializer->emitBytecode(output);
      output.pushGlobal(declareGlobal->name);
      break;
    }
    case StatementType::PRINT: {
      print->expression->emitBytecode(output);
      output.emitByte(OP_PRINT);
    }
  }
}

}