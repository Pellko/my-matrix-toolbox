#include "Statement.hh"
#include "src/compiler/Expression.hh"

namespace sciscript {

Statement::~Statement() {
  if(declareGlobal != nullptr) {
    delete declareGlobal->value;
    delete declareGlobal;
  }
  if(declareLocal != nullptr) {
    delete declareLocal->value;
    delete declareLocal;
  }
  if(print != nullptr) {
    delete print->expression;
    delete print;
  }

  if(expression != nullptr) {
    delete expression->expression;
    delete expression;
  }
  if(block != nullptr) {
    for(Statement* stmt : block->statements) {
      delete stmt;
    }
    delete block;
  }
}

void Statement::emitBytecode(CompilerOutput& output) {
  switch(type) {
    case StatementType::DECLARE_GLOBAL: {
      declareGlobal->value->emitBytecode(output);
      output.pushGlobal(declareGlobal->name);
      output.emitByte(OP_POP);
      break;
    }
    case StatementType::DECLARE_LOCAL: {
      declareLocal->value->emitBytecode(output);
      break;
    }
    case StatementType::PRINT: {
      print->expression->emitBytecode(output);
      output.emitByte(OP_PRINT);
      break;
    } 
    case StatementType::BLOCK: {
      for(Statement* stmt : block->statements) {
        stmt->emitBytecode(output);
      }
      for(int i=0;i<block->numLocals();i++) {
        output.emitByte(OP_POP);
      }
      break;
    }
    case StatementType::EXPRESSION: {
      expression->expression->emitBytecode(output);
      output.emitByte(OP_POP);
      break;
    }
  }
}

}