#include "ObjectMatrix.hh"

namespace sciscript {

ObjectMatrix::ObjectMatrix(int width, int height) : Object(ObjectType::MATRIX), width(width), height(height) {
  data = (Value*) malloc(width * height * sizeof(Value));
}

ObjectMatrix::~ObjectMatrix() {
  free(data);
}

void ObjectMatrix::setSize(int newWidth, int newHeight) {
  Value* newData = (Value*) malloc(newWidth * newHeight * sizeof(Value));

  for(int y=0;y<newHeight;y++) {
    for(int x=0;x<newWidth;x++) {
      int idx = x + y * width;
      int newIdx = x + y * newWidth;
      if(idx < width * height) {
        newData[newIdx] = data[idx];
      } else {
        newData[newIdx] = Value::fromDouble(0);
      }
    }
  }

  this->width = newWidth;
  this->height = newHeight;
  free(data);
  data = newData;
}

int ObjectMatrix::getWidth() {
  return width;
}

int ObjectMatrix::getHeight() {
  return height;
}

void ObjectMatrix::set(int x, int y, Value value) {
  data[x+y*width] = value;
}

Value ObjectMatrix::get(int x, int y) {
  return data[x + y * width];
}

void ObjectMatrix::print() {
  for(int y=0;y<height;y++) {
    for(int x=0;x<width;x++) {
      if(get(x,y).type == ValueType::NUMBER) {
        std::cout << get(x,y).as.number << " ";
      }
    }
    std::cout << std::endl;
  }
}

}