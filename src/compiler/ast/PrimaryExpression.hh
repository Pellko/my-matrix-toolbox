#ifndef _MY_MATRIX_TOOLBOX_COMPILER_AST_PRIMARY_EXPRESSION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_AST_PRIMARY_EXPRESSION_H_

#include "src/types/Literal.hh"
#include "Expression.hh"

namespace mymatrixtoolbox {

class ConstantExpression : public Expression {
 public:
  ConstantExpression(Literal literal) : literal(literal) {}
  ~ConstantExpression() {}

  void emitBytecode(Chunk& chunk) override;
  
 private:
  Literal literal;
};

class MatrixExpression : public Expression {
 public:
  MatrixExpression(int width, int height) : width(width), height(height) {
    expressions.resize(width * height);
  }
  ~MatrixExpression() {
    for(Expression* expression : expressions) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;
  void addExpression(int x, int y, Expression* expression) {
    expressions[x+y*width] = expression;
  }

 private:
  int width;
  int height;
  std::vector<Expression*> expressions;
};

class GroupExpression : public Expression {
 public:
  GroupExpression(Expression* expression) : expression(expression) {}
  ~GroupExpression() {
    if(expression != nullptr) {
      delete expression;
    }
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  Expression* expression;
};

class LocalExpression : public Expression {
 public:
  LocalExpression(VarRefType type, int index) : type(type), index(index) {}

  void emitBytecode(Chunk& chunk) override;

 private:
  VarRefType type;
  int index;
};

class ReadPropertyExpression : public Expression {
 public:
  ReadPropertyExpression(std::string propertyName, Expression* target) : propertyName(propertyName), target(target) {}
  ~ReadPropertyExpression() {
    if(target != nullptr) delete target;
  }

  void emitBytecode(Chunk& chunk) override;

 private:
  std::string propertyName;
  Expression* target;
};

}

#endif