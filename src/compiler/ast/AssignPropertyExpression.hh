#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_PROPERTY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_PROPERTY_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class AssignPropertyExpression : public Expression {
 public:
  AssignPropertyExpression(std::string propertyName, Expression* target, Expression* value) : propertyName(propertyName), target(target), value(value) {}
  ~AssignPropertyExpression() {
    if(value != nullptr) delete value;
    if(target != nullptr) delete target;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  std::string propertyName;
  Expression* target;
  Expression* value;
};

}

#endif
