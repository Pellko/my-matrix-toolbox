#ifndef _SCISCRIPT_COMPILER_AST_ASSIGN_VARIABLE_EXPRESSION_H_
#define _SCISCRIPT_COMPILER_AST_ASSIGN_VARIABLE_EXPRESSION_H_

#include "Expression.hh"
#include "DeclareVariableStatement.hh"

namespace sciscript {

class AssignVariableExpression : public Expression {
 public:
  AssignVariableExpression(DeclareVariableType type, int index, bool isUpvalue, Expression* value) : type(type), index(index), isUpvalue(isUpvalue), value(value) {}
  ~AssignVariableExpression() {
    if(value != nullptr) {
      delete value;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  DeclareVariableType type;
  int index;
  bool isUpvalue;
  Expression* value;
};

}

#endif
