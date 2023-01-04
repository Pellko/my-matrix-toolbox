#ifndef _MY_MATRIX_TOOLBOX_MATH_MATRIX_H_
#define _MY_MATRIX_TOOLBOX_MATH_MATRIX_H_

#include "src/types/Value.hh"
#include "src/types/Object.hh"
#include <iostream>

namespace mymatrixtoolbox {

class ObjectMatrix : public Object {
 public:
  ObjectMatrix(int width, int height);
  ~ObjectMatrix();

  void setSize(int width, int height);
  void set(int x, int y, Value value);
  void copy(ObjectMatrix& source);
  Value get(int x, int y);
  void getColumn(int k, ObjectMatrix& result);

  int getWidth();
  int getHeight();

  void add(ObjectMatrix& a);
  void subtract(ObjectMatrix& a);
  void multiply(double k);
  void multiply(ObjectMatrix& a, ObjectMatrix& result);
  void transpose();

  void rowEchelon(ObjectMatrix& b);
  void eigenvalues(ObjectMatrix& eigenvalues, ObjectMatrix& eigenvectors);
  void qrDecomposition(ObjectMatrix& Q, ObjectMatrix& R);
  
  static double dotP(ObjectMatrix& a, ObjectMatrix& b);

  bool isVector();
  bool isColumnVector();
  bool isRowVector();
  bool isNumeric();
  void print();
  std::string toString() override;

 private:
  int width;
  int height;
  Value* data;
};

}

#endif