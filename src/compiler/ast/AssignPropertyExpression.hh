#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_PROPERTY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_PROPERTY_EXPRESSION_H_

#include "Expression.hh"

namespace mymatrixtoolbox {

class AssignPropertyExpression : public Expression {
 public:
  AssignPropertyExpression(std::string propertyName, std::shared_ptr<Expression> target, std::shared_ptr<Expression> value) : propertyName(propertyName), target(target), value(value) {}
  ~AssignPropertyExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::string propertyName;
  std::shared_ptr<Expression> target;
  std::shared_ptr<Expression> value;
};

}

#endif
