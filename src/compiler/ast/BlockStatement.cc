#include "BlockStatement.hh"
#include "src/compiler/ast/Statement.hh"
#include "src/types/OpCode.hh"
#include "ReturnStatement.hh"

namespace mymatrixtoolbox {

void BlockStatement::emitBytecode(Chunk& chunk) {
  std::shared_ptr<ReturnStatement> returnStatement;

  for(std::shared_ptr<Statement> statement : statements) {
    if(statement->getType() == StatementType::RETURN) {
      returnStatement = std::static_pointer_cast<ReturnStatement>(statement);
      break;
    } else {
      statement->emitBytecode(chunk);
    }
  }

  if(returnStatement != nullptr) {
    returnStatement->emitBytecode(chunk);
    chunk.emitByte(OP_RETURN);
  }

  for(int i=locals.size()-1;i>=0;i--) {
    if(locals[i].hasClosureReference) {
      chunk.emitByte(OP_CLOSE_UPVALUE);
    } else {
      chunk.emitByte(OP_POP);
    }
  }
}

}