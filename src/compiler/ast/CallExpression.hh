#ifndef _SCISCRIPT_COMPILER_AST_CALL_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_CALL_EXPRESSION_H_

#include "Expression.hh"

namespace sciscript {

class CallExpression : public Expression {
 public:
  CallExpression(Expression* target, std::vector<Expression*> arguments) : target(target), arguments(arguments) {}
  ~CallExpression() {
    if(target != nullptr) {
      delete target;
    }
    for(Expression* arg : arguments) {
      delete arg;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* target;
  std::vector<Expression*> arguments;
};

}

#endif