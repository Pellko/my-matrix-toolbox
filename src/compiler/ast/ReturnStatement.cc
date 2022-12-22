#include "ReturnStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void ReturnStatement::emitBytecode(Chunk& chunk) {
  if(expression != nullptr) {
    expression->emitBytecode(chunk);
  }
}

}