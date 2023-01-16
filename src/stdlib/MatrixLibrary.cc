#include "MatrixLibrary.hh"
#include "src/math/ObjectMatrix.hh"

namespace mymatrixtoolbox {
namespace stdlib {

// TODO: There is a eventual GC bug here where temporary matrices can be collected if GC were to be triggered from allocate object
static Value solve(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args) {
  ObjectMatrix* a = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* b = static_cast<ObjectMatrix*>(args[1].as.object);

  ObjectMatrix* result = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  result->setSize(1, b->getHeight());
  result->copy(*b);
  a->rowEchelon(*result);

  return Value::fromObject(result);
}

// TODO: There is a eventual GC bug here where temporary matrices can be collected if GC were to be triggered from allocate object
static Value eig(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args) {
  ObjectMatrix* a = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* eigenvalues = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  ObjectMatrix* eigenvectors = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  a->eigenvalues(*eigenvalues, *eigenvectors);

  ObjectMatrix* result = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  result->setSize(1, 2);
  result->set(0, 0, Value::fromObject(eigenvalues));
  result->set(0, 1, Value::fromObject(eigenvectors));

  return Value::fromObject(result);
}

// TODO: type checking
static Value zeros(ExecutionContext* context, VirtualMachine* vm, std::vector<Value> args) {
  int width = args[0].as.number;
  int height = args[1].as.number;
  ObjectMatrix* result = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  result->setSize(width, height);
  return Value::fromObject(result);
}

Library createMatrixLibrary() {
  Library library;

  library.registerObjectMethod("solve", ObjectType::MATRIX, &solve);
  library.registerObjectMethod("eig", ObjectType::MATRIX, &eig);
  
  library.registerFunction("zeros", &zeros);

  return library;
}

}
}