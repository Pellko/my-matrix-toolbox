#include "FunctionStatement.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/types/OpCode.hh"

namespace sciscript {

void FunctionStatement::emitBytecode(Chunk& chunk) {
  // Emit function body in functions own scope
  block->emitBytecode(compilerScope->chunk);

  // Turn function into closure object
  Chunk* parentChunk = nullptr;
  if(parentScope != nullptr) {
    parentChunk = &parentScope->chunk;
  } else {
    parentChunk = &chunk;
  }
  
  parentChunk->emitByte(OP_CLOSURE);
  parentChunk->emitDynamicBytes(functionIndex);

  for(const Upvalue& upvalue : upvalues) {
    parentChunk->emitByte(upvalue.isLocal ? 1 : 0);
    parentChunk->emitDynamicBytes(upvalue.index);
  }
    
  if(isGlobal) {
    parentChunk->emitByte(OP_SET_GLOBAL);
    parentChunk->emitDynamicBytes(globalIndex);
    parentChunk->emitByte(OP_POP);
  }
}

std::vector<Argument> FunctionStatement::readArgumentList(ParserTool& parserTool) {
  std::vector<Argument> list;
  
  while(!parserTool.empty() && parserTool.peek()->type != Token::Kind::RPAREN) {
    if(parserTool.peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected variable name");
    }
    Token* name = parserTool.get();
    
    if(parserTool.empty()) {
      throw new SyntaxException("Unexpected ending of variable list");
    }
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::COMMA) {
      parserTool.get();
    }

    list.push_back(Argument{
      .name = name->text
    });
  }

  return list;
}

}