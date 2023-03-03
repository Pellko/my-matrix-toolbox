#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_MATCH_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_MATCH_EXPRESSION_H_

#include "Expression.hh"
#include "BinaryExpression.hh"
#include <vector>

namespace mymatrixtoolbox {

class MatchExpression : public Expression {
 public:
  MatchExpression(std::shared_ptr<Expression> value) : value(value) {}
  ~MatchExpression() {}

  void emitBytecode(Chunk& chunk) override;

  void addResult(std::vector<std::shared_ptr<Expression>> targets, std::shared_ptr<Expression> result) {
    this->targets.push_back(targets);
    this->results.push_back(result);
  }

 private:
  std::shared_ptr<Expression> value;
  std::vector<std::vector<std::shared_ptr<Expression>>> targets;
  std::vector<std::shared_ptr<Expression>> results;

  std::shared_ptr<BinaryExpression> buildExpression(std::shared_ptr<Expression>& value, std::vector<std::shared_ptr<Expression>>& targets, int currIndex = 0);
};

}

#endif