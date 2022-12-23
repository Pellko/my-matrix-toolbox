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

void ObjectMatrix::getColumn(int k, ObjectMatrix& result) {
  result.setSize(1, height);
  for(int y=0;y<height;y++) {
    result.set(0, y, get(k, y));
  }
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

void ObjectMatrix::add(ObjectMatrix& a) {
  if(a.getWidth() != width || a.getHeight() != height) {
    // TODO: throw exception
    return;
  }

  for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      set(x, y, Value::fromDouble(get(x, y).as.number + a.get(x, y).as.number));
    }
  }
}

void ObjectMatrix::subtract(ObjectMatrix& a) {
  if(a.getWidth() != width || a.getHeight() != height) {
    // TODO: throw exception
    return;
  }

  for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      set(x, y, Value::fromDouble(get(x, y).as.number - a.get(x, y).as.number));
    }
  }
}

void ObjectMatrix::multiply(double k) {
  for(int x=0;x<width;x++) {
    for(int y=0;y<height;y++) {
      set(x,y, Value::fromDouble(get(x,y).as.number * k));
    }
  }
}

void ObjectMatrix::multiply(ObjectMatrix& a, ObjectMatrix& result) {
  if(width != a.getHeight()) {
    // TODO: throw exception
    return;
  }
  result.setSize(a.getWidth(), height);

  for(int x=0;x<a.getWidth();x++) {
    for(int y=0;y<height;y++) {
      double value = 0;
      for(int k=0;k<width;k++) {
        value += get(k, y).as.number * a.get(x, k).as.number;
      }
      result.set(x,y,Value::fromDouble(value));
    }
  }
}

void ObjectMatrix::transpose() {
  for(int y=0;y<height;y++) {
    for(int x=0;x<y;x++) {
      Value t = get(y,x);
      set(y, x, get(x, y));
      set(x, y, t);
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

void ObjectMatrix::eigenvalues(ObjectMatrix& eigenvalues, ObjectMatrix& eigenvectors) {
  if(width != height) {
    // TODO: make sure matrix is square
    return;
  }

  // Initialize
  int iterations = 10;
  ObjectMatrix B(width, height);
  B.copy(*this);
  ObjectMatrix U(width, height);
  for(int x=0;x<width;x++) {
    U.set(x,x, Value::fromDouble(1));
  }

  // Iterate
  for(int i=0;i<iterations;i++) {
    ObjectMatrix Q(width, height);
    ObjectMatrix R(width, height);
    B.qrDecomposition(Q, R);
    R.multiply(Q, B);
    ObjectMatrix temp(width, height);
    temp.copy(U);
    temp.multiply(Q, U);
  }

  eigenvalues.setSize(width, height);
  for(int i=0;i<width;i++) {
    eigenvalues.set(i,i,B.get(i,i));
  }
  eigenvectors.setSize(width, height);
  eigenvectors.copy(U);
}

void ObjectMatrix::qrDecomposition(ObjectMatrix& Q, ObjectMatrix& R) {
  // Initialize result matrices
  Q.setSize(width, height);
  R.setSize(width, height);

  ObjectMatrix workingMatrix(width, height);
  workingMatrix.copy(*this);

  // Perform Gram-Schmidt orthonormalization on the columns of our matrix
  for(int x=1;x<width;x++) {
    // Get last column
    ObjectMatrix prevCol(1, height);
    workingMatrix.getColumn(x - 1, prevCol);

    // Get current column
    ObjectMatrix currCol(1, height);
    workingMatrix.getColumn(x, currCol);

    double projFactor = dotP(currCol, prevCol) / dotP(prevCol, prevCol);
    prevCol.multiply(projFactor);
    currCol.subtract(prevCol);

    // Insert column
    for(int y=0;y<height;y++) {
      workingMatrix.set(x, y, currCol.get(0, y));
    }
  }

  // Normalize columns
  for(int x=0;x<width;x++) {
    double length = 0;
    for(int y=0;y<height;y++) {
      length += workingMatrix.get(x, y).as.number * workingMatrix.get(x, y).as.number;
    }
    length = sqrt(length);

    for(int y=0;y<height;y++) {
      workingMatrix.set(x, y, Value::fromDouble(workingMatrix.get(x,y).as.number / length));
    }
  }

  // The working matrix is Q
  Q.copy(workingMatrix);

  // Calculate R
  ObjectMatrix transposedQ(width, height);
  transposedQ.copy(Q);
  transposedQ.transpose();
  transposedQ.multiply(*this, R);
}

double ObjectMatrix::dotP(ObjectMatrix& a, ObjectMatrix& b) {
  // TODO: check that everything is numeric
  if(!a.isVector() || !b.isVector()) {
    // TODO: throw exception
    return -1;
  }

  int aSize = a.isColumnVector() ? a.height : a.width;
  int bSize = b.isColumnVector() ? b.height : b.width;
  if(aSize != bSize) {
    // TODO: throw exception
    return -1;
  }

  double result = 0;
  for(int k=0;k<aSize;k++) {
    double f = 1;
    f *= a.isColumnVector() ? a.get(0, k).as.number : a.get(k, 0).as.number;
    f *= b.isColumnVector() ? b.get(0, k).as.number : b.get(k, 0).as.number;
    result += f;
  }

  return result;
}

bool ObjectMatrix::isVector() {
  return isColumnVector() || isRowVector();
}

bool ObjectMatrix::isColumnVector() {
  return width == 1;
}

bool ObjectMatrix::isRowVector() {
  return height == 1;
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