#ifndef _SCISCRIPT_COMPILER_AST_CALL_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_CALL_EXPRESSION_H_

#include "Expression.hh"

namespace sciscript {

class CallExpression : public Expression {
 public:
  CallExpression(Expression* target) : target(target) {}
  ~CallExpression() {
    if(target != nullptr) {
      delete target;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* target;
};

}

#endif