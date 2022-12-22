#include "ParserTool.hh"
#include "src/types/CompilerScope.hh"
#include "src/types/Object.hh"
#include "src/types/Upvalue.hh"
#include "src/compiler/ast/BlockStatement.hh"

namespace mymatrixtoolbox {

Token* ParserTool::peek(int n) {
  if(position == tokens.size()) {
    return nullptr;
  } else {
    return &tokens[position + n];
  }
}

Token* ParserTool::get() {
  Token* currToken = &tokens[position];
  position++;
  return currToken;
}

bool ParserTool::empty() {
  return position == tokens.size();
}

bool ParserTool::require(int n) {
  return position < tokens.size() - n;
}

bool ParserTool::expressionNext() {
  Token::Kind type = peek()->type;

  if(type == Token::Kind::NUMBER || type == Token::Kind::MINUS || type == Token::Kind::LPAREN) {
    return true;
  }

  if(type == Token::Kind::IDENTIFIER && peek()->text != "print") {
    return true;
  }

  return false;
}

void ParserTool::beginScope() {
  scopeLevel++;
}

void ParserTool::endScope() {
  scopeLevel--;

  // Remove locals
  std::vector<int> removeIndexes;
  for(int i=0;i<locals.size();i++) {
    if(locals[i].depth > scopeLevel) {
      removeIndexes.push_back(i);
    }
  }

  for(int i=removeIndexes.size()-1;i>=0;i--) {
    locals.erase(locals.begin() + removeIndexes[i]);
  }
}

void ParserTool::beginFunction(std::string name) {
  // Setup function scope tree
  CompilerScope* compilerScope = new CompilerScope;
  compilerScope->name = name;
  compilerScope->chunk.numUpvalues = 0;

  if(compilerScopeTree == nullptr) {
    compilerScopeTree = new CompilerScopeTree;
    compilerScopeTree->current = compilerScope;
    compilerScopeTree->depth = scopeLevel;
    compilerScopeTree->localsOffset = scopeLevel > 0 ? locals.size() + 1 : locals.size();
  } else {
    CompilerScopeTree* oldTreeNode = compilerScopeTree;
    compilerScopeTree = new CompilerScopeTree;
    compilerScopeTree->parent = oldTreeNode;
    compilerScopeTree->current = compilerScope;
    compilerScopeTree->depth = scopeLevel;
    compilerScopeTree->localsOffset = scopeLevel > 0 ? locals.size() + 1 : locals.size();
  }

  // Store function compiler scope
  functionCompilerScopes.push_back(compilerScopeTree->current);

  // Register function local
  if(scopeLevel > 0) {
    locals.push_back(Local{
      .name = compilerScopeTree->current->name,
      .depth = scopeLevel,
      .hasClosureReference = false,
    });
  }
}

void ParserTool::beginLambda() {
  // Setup function scope tree
  CompilerScope* compilerScope = new CompilerScope;
  compilerScope->name = "lambda";
  compilerScope->chunk.numUpvalues = 0;

  if(compilerScopeTree == nullptr) {
    compilerScopeTree = new CompilerScopeTree;
    compilerScopeTree->current = compilerScope;
    compilerScopeTree->depth = scopeLevel;
    compilerScopeTree->localsOffset = locals.size();
  } else {
    CompilerScopeTree* oldTreeNode = compilerScopeTree;
    compilerScopeTree = new CompilerScopeTree;
    compilerScopeTree->parent = oldTreeNode;
    compilerScopeTree->current = compilerScope;
    compilerScopeTree->depth = scopeLevel;
    compilerScopeTree->localsOffset = locals.size();
  }

  // Store function compiler scope
  functionCompilerScopes.push_back(compilerScopeTree->current);
}

void ParserTool::endFunction() {
  // Pop tree
  if(compilerScopeTree->parent != nullptr) {
    CompilerScopeTree* oldTreeNode = compilerScopeTree;
    compilerScopeTree = oldTreeNode->parent;
    delete oldTreeNode;
  } else {
    compilerScopeTree = nullptr;
  }
}

std::pair<VarRefType, int> ParserTool::findIdentifier(std::string name) {
  // Check if there is a local with this name
  int localsOffset = 0;
  int currentFunctionScope = 0;
  if(compilerScopeTree != nullptr) {
    currentFunctionScope = compilerScopeTree->depth;
    localsOffset = compilerScopeTree->localsOffset;
  }

  int localIndex = -1;
  for(int i=locals.size()-1;i>=0;i--) {
    if(locals[i].name == name && locals[i].depth <= scopeLevel && locals[i].depth > currentFunctionScope) {
      localIndex = i;
      break;
    }
  }
  
  if(localIndex != -1) {
    return std::make_pair(VarRefType::LOCAL, localIndex - localsOffset);
  }

  // Check for locals outside of function scope if we aren't in top level function => Upvalues
  if(currentFunctionScope >= 1) {
    localIndex = -1;
    for(int i=locals.size()-1;i>=0;i--) {
      if(locals[i].name == name && locals[i].depth <= currentFunctionScope && locals[i].depth <= scopeLevel) {
        localIndex = i;
        break;
      }
    }

    if(localIndex != -1) {
      return std::make_pair(VarRefType::UPVALUE, localIndex);
    }
  }

  // Check if there is a global with this name
  int globalIndex = -1;
  for(int i=0;i<globals.size();i++) {
    if(globals[i] == name) {
      globalIndex = i;
      break;
    }
  }
  return std::make_pair(VarRefType::GLOBAL, globalIndex);
}

int ParserTool::registerGlobal(std::string name) {
  globals.push_back(name);
  return globals.size() - 1;
}

int ParserTool::registerLocal(std::string name) {
  locals.push_back(Local{
    .name = name,
    .depth = scopeLevel,
    .hasClosureReference = false,
  });

  int localsOffset = 0;
  if(compilerScopeTree != nullptr) {
    localsOffset = compilerScopeTree->localsOffset;
  }
  return (locals.size() - 1) - localsOffset;
}

int ParserTool::registerUpvalue(std::string name) {
  return resolveUpvalue(compilerScopeTree, name); 
}

int ParserTool::resolveUpvalue(CompilerScopeTree* scope, std::string name) {
  int parentDepth = 0;
  if(scope->parent != nullptr) {
    parentDepth = scope->parent->depth;
  }

  int localIndex = -1;
  for(int i=locals.size()-1;i>=0;i--) {
    if(locals[i].name == name && locals[i].depth == parentDepth + 1) {
      localIndex = i;
      break;
    }
  }
  if(localIndex != -1) {
    locals[localIndex].hasClosureReference = true;
    return addUpvalue(scope, localIndex, true);
  }

  int upvalue = resolveUpvalue(scope->parent, name);
  if(upvalue != -1) {
    return addUpvalue(scope, upvalue, false);
  }

  return -1;
}

int ParserTool::addUpvalue(CompilerScopeTree* scope, int index, bool isLocal) {
  // Check if we already are closing over this local
  int upvalueCount = scope->current->upvalues.size();
  for(int i=0;i<upvalueCount;i++) {
    Upvalue& upvalue = scope->current->upvalues[i];
    if(upvalue.index == index && upvalue.isLocal == isLocal) {
      return i;
    }
  }

  scope->current->upvalues.push_back(Upvalue{
    .index = index,
    .isLocal = isLocal,
  });
  scope->current->chunk.numUpvalues++;
  return upvalueCount;
}

void ParserTool::storeLocalsInBlockStatement(BlockStatement* statement) {
  for(int i=0;i<locals.size();i++) {
    if(locals[i].depth >= scopeLevel) {
      statement->addLocal(locals[i]);
    }
  }
}

}