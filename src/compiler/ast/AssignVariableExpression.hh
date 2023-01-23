#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_VARIABLE_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_ASSIGN_VARIABLE_EXPRESSION_H_

#include "Expression.hh"
#include "DeclareVariableStatement.hh"

namespace mymatrixtoolbox {

class AssignVariableExpression : public Expression {
 public:
  AssignVariableExpression(DeclareVariableType type, int index, bool isUpvalue, std::shared_ptr<Expression> value) : type(type), index(index), isUpvalue(isUpvalue), value(value) {}
  ~AssignVariableExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  DeclareVariableType type;
  int index;
  bool isUpvalue;
  std::shared_ptr<Expression> value;
};

}

#endif
