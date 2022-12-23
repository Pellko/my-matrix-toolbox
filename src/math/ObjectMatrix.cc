#include "ObjectMatrix.hh"

namespace mymatrixtoolbox {

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

void ObjectMatrix::copy(ObjectMatrix& source) {
  if(source.getWidth() != width || source.getHeight() != height) {
    return;
  }
  for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      set(x,y, source.get(x, y));
    }
  }
}

void ObjectMatrix::rowEchelon(ObjectMatrix& b) {
  if(!isNumeric()) {
    return;
  }

  // TODO: check if matrix is numeric
  if(b.getHeight() != width) {
    return;
  }

  ObjectMatrix matrix(width, height);
  matrix.copy(*this);

  int h=0;
  int k=0;

  while(h < height && k < width) {
    // Find the pivot
    int pivotRow = -1;
    int pivotVal = -1;
    for(int i=h;i<height;i++) {
      if(pivotRow == -1) {
        pivotRow = i;
        pivotVal = abs(matrix.get(k, i).as.number);
      } else {
        if(abs(matrix.get(k, i).as.number) > pivotVal) {
          pivotRow = i;
          pivotVal = abs(matrix.get(k, i).as.number);
        }
      }
    }
    
    if(matrix.get(k, pivotRow).as.number == 0) {
      k += 1;
    } else {
      // Swap pivot row to h
      for(int x=0;x<width;x++) {
        Value temp = matrix.get(x, h);
        matrix.set(x, h, matrix.get(x, pivotRow));
        matrix.set(x, pivotRow, temp);
      }

      // Swap row for result matrix
      Value temp = b.get(0, h);
      b.set(0, h, b.get(0, pivotRow));
      b.set(0, pivotRow, temp);

      for(int i=h+1;i<height;i++) {
        double f = matrix.get(k, i).as.number / matrix.get(k, h).as.number;
        matrix.set(k, i, Value::fromDouble(0));

        for(int j=k+1;j<width;j++) {
          matrix.set(j, i, Value::fromDouble(matrix.get(j, i).as.number - (matrix.get(j, h).as.number * f)));
        }
        b.set(0, i, Value::fromDouble(b.get(0, i).as.number - b.get(0, h).as.number * f));
      }

      h++;
      k++;
    }
  }

  // Normalize leading coefficient in all rows
  for(int y=0;y<height;y++) {
    double factor = matrix.get(y, y).as.number;
    for(int x=y;x<width;x++) {
      matrix.set(x, y, Value::fromDouble(matrix.get(x, y).as.number / factor));
    }
    b.set(0, y, Value::fromDouble(b.get(0, y).as.number / factor));
  }

  // Backsubstitute
  for(int y=height-1;y>=0;y--) {
    if(y-1 >= 0) {
      for(int j=y-1;j>=0;j--) {
        b.set(
          0, j,
          Value::fromDouble(
            b.get(0, j).as.number - (b.get(0, y).as.number * matrix.get(y, j).as.number)
          )
        );
        matrix.set(y, j, Value::fromDouble(0));
      }
    }
  }
}

bool ObjectMatrix::isNumeric() {
  bool isNumeric = true;
  for(int i=0;i<width*height;i++) {
    if(data[i].type != ValueType::NUMBER) {
      isNumeric = false;
      break;
    }
  }
  return isNumeric;
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