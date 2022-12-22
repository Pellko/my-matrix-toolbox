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

  int getWidth();
  int getHeight();

  void rowEchelon(ObjectMatrix& b);

  bool isNumeric();
  void print();
 private:
  int width;
  int height;
  Value* data;
};

}

#endif