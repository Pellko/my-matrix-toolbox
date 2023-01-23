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
  ~MatrixExpression() {}

  void emitBytecode(Chunk& chunk) override;
  void addExpression(int x, int y, std::shared_ptr<Expression> expression) {
    expressions[x+y*width] = expression;
  }

 private:
  int width;
  int height;
  std::vector<std::shared_ptr<Expression>> expressions;
};

class MapExpression : public Expression {
 public:
  MapExpression() {}
  ~MapExpression() {}

  void emitBytecode(Chunk& chunk) override;
  void addValue(std::shared_ptr<Expression> name, std::shared_ptr<Expression> value) {
    values.push_back(std::make_pair(name, value));
  }

 private:
  std::vector<std::pair<std::shared_ptr<Expression>, std::shared_ptr<Expression>>> values;
};

class GroupExpression : public Expression {
 public:
  GroupExpression(std::shared_ptr<Expression> expression) : expression(expression) {}
  ~GroupExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::shared_ptr<Expression> expression;
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
  ReadPropertyExpression(std::string propertyName, std::shared_ptr<Expression> target) : propertyName(propertyName), target(target) {}
  ~ReadPropertyExpression() {}

  void emitBytecode(Chunk& chunk) override;

 private:
  std::string propertyName;
  std::shared_ptr<Expression> target;
};

}

#endif