#include "Expression.hh"
#include "src/types/ArithmeticType.hh"
#include "src/types/CompilerOutput.hh"

namespace sciscript {

PrimaryExpression::~PrimaryExpression() {
  delete group;
}

void PrimaryExpression::emitBytecode(CompilerOutput& output) {
  switch(type) {
    case PrimaryType::CONSTANT:
      output.pushConstant(value);
      break;
    case PrimaryType::GROUP:
      group->emitBytecode(output);
      break;
    case PrimaryType::GLOBAL_VARIABLE:
      output.readGlobal(globalVariableIndex);
      break;
    case PrimaryType::LOCAL_VARIABLE:
      output.readLocal(localVariableIndex);
      break;
  }
}

Expression::~Expression() {
  if(arithmetic != nullptr) {
    delete arithmetic->left;
    delete arithmetic->right;
    delete arithmetic;
  }
  if(negation != nullptr) {
    delete negation->child;
    delete negation;
  }
  if(comparison != nullptr) {
    delete comparison->left;
    delete comparison->right;
    delete comparison;
  }
  if(setLocal != nullptr) {
    delete setLocal->value;
    delete setLocal;
  }
  if(setGlobal != nullptr) {
    delete setGlobal->value;
    delete setGlobal;
  }
  if(ternary != nullptr) {
    delete ternary->condition;
    delete ternary->trueExpression;
    delete ternary->elseExpression;
  }
  delete primary;
}

void Expression::emitBytecode(CompilerOutput& output) {
  if(type == ExpressionType::ARITHMETIC) {
    arithmetic->left->emitBytecode(output);
    arithmetic->right->emitBytecode(output);
    switch(arithmetic->type) {
      case ArithmeticType::PLUS:
        output.emitByte(OP_ADD);
        break;
      case ArithmeticType::MINUS:
        output.emitByte(OP_MINUS);
        break;
      case ArithmeticType::MULT:
        output.emitByte(OP_MULT);
        break;
      case ArithmeticType::DIV:
        output.emitByte(OP_DIV);
        break;
      case ArithmeticType::MOD:
        output.emitByte(OP_MOD);
        break;
    }
  }

  if(type == ExpressionType::COMPARISON) {
    comparison->left->emitBytecode(output);
    comparison->right->emitBytecode(output);
    switch(comparison->type) {
      case ComparisonType::EQUALS:
        output.emitByte(OP_EQUALS);
        break;
      case ComparisonType::LT:
        output.emitByte(OP_LT);
        break;
      case ComparisonType::GT:
        output.emitByte(OP_LEQ);
        output.emitByte(OP_NEG);
        break;
      case ComparisonType::LEQ:
        output.emitByte(OP_LEQ);
        break;
      case ComparisonType::GEQ:
        output.emitByte(OP_LT);
        output.emitByte(OP_NEG);
        break;
    }
  }

  if(type == ExpressionType::NEGATION) {
    negation->child->emitBytecode(output);
    output.emitByte(OP_NEG);
  }

  if(type == ExpressionType::PRIMARY) {
    primary->emitBytecode(output);
  }

  if(type == ExpressionType::SET_LOCAL) {
    setLocal->value->emitBytecode(output);
    output.setLocal(setLocal->name, setLocal->index);
  }

  if(type == ExpressionType::SET_GLOBAL) {
    setGlobal->value->emitBytecode(output);
    output.setGlobal(setGlobal->name);
  }

  if(type == ExpressionType::TERNARY) {
    ternary->condition->emitBytecode(output);
    output.emitByte(OP_JUMP_FALSE);

    // Emit true statement
    int trueStatementStart = output.bytecode.size();
    ternary->trueExpression->emitBytecode(output);
    int trueStatementLength = output.bytecode.size() - trueStatementStart;

    // Emit else statement if it exists
    if(ternary->elseExpression != nullptr) {
      int elseStatementStart = output.bytecode.size();
      ternary->elseExpression->emitBytecode(output);
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
  }
}

}