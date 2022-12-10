#include "Statement.hh"
#include "src/compiler/Expression.hh"
#include "src/types/OpCode.hh"

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
  if(ifStatement != nullptr) {
    delete ifStatement->condition;
    delete ifStatement->trueStatement;
    delete ifStatement->elseStatement;
    delete ifStatement;
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
    case StatementType::IF: {
      // Emit condition and jump
      ifStatement->condition->emitBytecode(output);
      output.emitByte(OP_JUMP_FALSE);

      // Emit true statement
      int trueStatementStart = output.bytecode.size();
      ifStatement->trueStatement->emitBytecode(output);
      int trueStatementLength = output.bytecode.size() - trueStatementStart;

      // Emit else statement if it exists
      if(ifStatement->elseStatement != nullptr) {
        int elseStatementStart = output.bytecode.size();
        ifStatement->elseStatement->emitBytecode(output);
        int elseStatementEnd = output.bytecode.size();
        std::vector<uint8_t> bytes = output.generateDynamicBytes(elseStatementEnd - elseStatementStart);
        trueStatementLength += bytes.size() + 1; // Add OP_JUMP + argument
        output.bytecode.insert(output.bytecode.begin() + elseStatementStart, OP_JUMP);
        for(int i=1;i<=bytes.size();i++) {
          output.bytecode.insert(output.bytecode.begin() + elseStatementStart + i, bytes[i-1]);
        }
      }

      // Set OP_JUMP_FALSE argument
      std::vector<uint8_t> bytes = output.generateDynamicBytes(trueStatementLength);
      for(int i=0;i<bytes.size();i++) {
        output.bytecode.insert(output.bytecode.begin() + trueStatementStart + i, bytes[i]);
      }
      break;
    }
  }
}

}