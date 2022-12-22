#include "PrintStatement.hh"
#include "src/types/OpCode.hh"

namespace mymatrixtoolbox {

void PrintStatement::emitBytecode(Chunk& chunk) {
  expression->emitBytecode(chunk);
  chunk.emitByte(OP_PRINT);
}

}