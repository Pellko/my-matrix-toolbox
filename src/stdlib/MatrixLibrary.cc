#include "MatrixLibrary.hh"
#include "src/math/ObjectMatrix.hh"

namespace mymatrixtoolbox {
namespace stdlib {

static Value solve(VirtualMachine* vm, std::vector<Value> args) {
  ObjectMatrix* a = static_cast<ObjectMatrix*>(args[0].as.object);
  ObjectMatrix* b = static_cast<ObjectMatrix*>(args[1].as.object);

  ObjectMatrix* result = static_cast<ObjectMatrix*>(vm->allocateObject(ObjectType::MATRIX));
  result->setSize(1, b->getHeight());
  result->copy(*b);
  a->rowEchelon(*result);

  return Value::fromObject(result);
}

Library createMatrixLibrary() {
  Library library;

  library.registerObjectMethod("solve", ObjectType::MATRIX, &solve);

  return library;
}

}
}