#include "MatchExpression.hh"
#include "IfStatement.hh"
#include "PrimaryExpression.hh"
#include "BinaryExpression.hh"
#include "ControlFlowBuilder.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void MatchExpression::emitBytecode(Chunk& chunk) {
  // Match statement is a NO-OP if we have no targets defined
  if(targets.size() == 0) {
    ConstantExpression node(Literal::nil());
    node.emitBytecode(chunk);
    return;
  }

  // Emit proper control flow, here we have an expression so every part of the control flow is expected to return some value
  ControlFlowBuilder builder = ControlFlowBuilder::begin();
  for(int i=0;i<targets.size();i++) {
    builder = builder.addClause(
      [=](Chunk& chunk) {
        buildExpression(value, targets[i])->emitBytecode(chunk);
      }, 
      [=](Chunk& chunk) {
        results[i]->emitBytecode(chunk);
      }
    );
  }
  builder.emitBytecode(chunk);
}

std::shared_ptr<BinaryExpression> MatchExpression::buildExpression(std::shared_ptr<Expression>& value, std::vector<std::shared_ptr<Expression>>& targets, int currIndex) {
  if(targets.size() == 1) {
    return std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[0], value);
  }

  if(targets.size() == 2) {
    std::shared_ptr<BinaryExpression> eq1 = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[0], value);
    std::shared_ptr<BinaryExpression> eq2 = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[1], value);
    return std::make_shared<BinaryExpression>(BinaryOperation::LOGICAL_OR, eq1, eq2);
  }

  if(currIndex == targets.size() - 2) {
    std::shared_ptr<BinaryExpression> eq1 = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[currIndex + 0], value);
    std::shared_ptr<BinaryExpression> eq2 = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[currIndex + 1], value);
    return std::make_shared<BinaryExpression>(BinaryOperation::LOGICAL_OR, eq1, eq2);
  } else {
    std::shared_ptr<BinaryExpression> eq1 = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, targets[currIndex], value);
    std::shared_ptr<BinaryExpression> eq2 = buildExpression(value, targets, currIndex + 1);
    return std::make_shared<BinaryExpression>(BinaryOperation::LOGICAL_OR, eq1, eq2);
  }
}

}