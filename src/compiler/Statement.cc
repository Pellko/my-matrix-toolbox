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
  if(forStatement != nullptr) {
    delete forStatement->initializer;
    delete forStatement->condition;
    delete forStatement->incrementor;
    delete forStatement->body;
    delete forStatement;
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
    case StatementType::FOR: {
      if(forStatement->initializer != nullptr) {
        forStatement->initializer->emitBytecode(output);
      }

      int conditionStart = output.bytecode.size();
      if(forStatement->condition != nullptr) {
        forStatement->condition->emitBytecode(output);
      }
      int conditionLength = output.bytecode.size() - conditionStart;
      output.emitByte(OP_JUMP_FALSE);

      int bodyStart = output.bytecode.size();
      forStatement->body->emitBytecode(output);
      int bodyLength = output.bytecode.size() - bodyStart;

      int incrementorStart = output.bytecode.size();
      if(forStatement->incrementor != nullptr) {
        forStatement->incrementor->emitBytecode(output);
        output.emitByte(OP_POP);
      }
      int incrementLength = output.bytecode.size() - incrementorStart;
      output.emitByte(OP_LOOP);

      int jumpSize = conditionLength + bodyLength + incrementLength;
      std::vector<uint8_t> bytes = output.generateDynamicBytes(jumpSize);
      jumpSize += bytes.size();
      if(output.generateDynamicBytes(jumpSize).size() != bytes.size()) {
        int oldSize = bytes.size();
        bytes = output.generateDynamicBytes(jumpSize);
        jumpSize -= oldSize;
        jumpSize += bytes.size();
      }  else {
        bytes = output.generateDynamicBytes(jumpSize);
      }
      output.emitDynamicBytes(jumpSize);
      for(int i=0;i<bytes.size();i++) {
        output.bytecode.insert(output.bytecode.begin() + conditionStart + conditionLength + 1 + i, bytes[i]);
      }
      if(forStatement->initializer != nullptr && forStatement->initializer->type == StatementType::DECLARE_LOCAL) {
        output.emitByte(OP_POP);
      }
      break;
    }
  }
}

}