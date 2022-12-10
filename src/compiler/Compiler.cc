#include "Compiler.hh"
#include "src/compiler/Expression.hh"
#include "src/compiler/Statement.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/types/CompilerOutput.hh"

namespace sciscript {

Compiler::Compiler(std::vector<Token>& tokens) : tokens(tokens) {}

CompilerOutput& Compiler::compile() {
  position = 0;

  while(!empty()) {
    statement(output);
  }

  return output;
}

Token* Compiler::peek(int n) {
  if(position == tokens.size()) {
    return nullptr;
  } else {
    return &tokens[position + n];
  }
}

Token* Compiler::get() {
  Token* currToken = &tokens[position];
  position++;
  return currToken;
}

bool Compiler::empty() {
  return position == tokens.size();
}

bool Compiler::require(int n) {
  return position < tokens.size() - n;
}

bool Compiler::expressionNext() {
  Token::Kind type = peek()->type;

  if(type == Token::Kind::NUMBER || type == Token::Kind::MINUS || type == Token::Kind::LPAREN) {
    return true;
  }

  if(type == Token::Kind::IDENTIFIER && peek()->text != "print") {
    return true;
  }

  return false;
}

void Compiler::statement(CompilerOutput& output) {
  // Parse statement
  Statement* parsedStatement = readStatement();
  parsedStatement->emitBytecode(output);
  delete parsedStatement;
}

void Compiler::expression(CompilerOutput& output) {
  // Parse expression
  Expression* parsedExpression = readExpression();

  // Compile expression
  parsedExpression->emitBytecode(output);

  delete parsedExpression;
}

Statement* Compiler::readStatement() {
  Token* next = peek();

  if(next->type == Token::Kind::LBRACE) {
    currScopeLevel++;
    std::vector<Statement*> statements = readBlock();
    Statement* stmt = new Statement(StatementType::BLOCK);
    stmt->block = new Statement::Block;
    stmt->block->statements = statements;
    stmt->block->depth = currScopeLevel;
    currScopeLevel--;

    std::vector<int> removeIndexes;
    for(int i=0;i<locals.size();i++) {
      if(locals[i].depth > currScopeLevel) {
        removeIndexes.push_back(i);
      }
    }

    for(int i=removeIndexes.size()-1;i>=0;i--) {
      locals.erase(locals.begin() + removeIndexes[i]);
    }

    return stmt;
  }

  // Expression statement
  if(expressionNext()) {
    Statement* stmt = new Statement(StatementType::EXPRESSION);
    stmt->expression = new Statement::ExpressionStatement;
    stmt->expression->expression = readExpression();

    if(peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected ; to end statement");
    }
    get();

    return stmt;
  }

  // If statement
  if(next->type == Token::Kind::IF) {
    get();

    if(empty() || peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected (");
    }
    get();

    Expression* condition = readExpression();

    if(empty() || peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected )");
    }
    get();

    if(empty()) {
      throw new SyntaxException("Expected statement");
    }

    Statement* trueStatement = readStatement();
    Statement* elseStatement = nullptr;

    if(!empty() && peek()->type == Token::Kind::ELSE) {
      get();
      elseStatement = readStatement();
    }

    Statement* node = new Statement(StatementType::IF);
    node->ifStatement = new Statement::If;
    node->ifStatement->condition = condition;
    node->ifStatement->trueStatement = trueStatement;
    node->ifStatement->elseStatement = elseStatement;
    
    return node;
  }

  // Declare global or local variable
  if(next->type == Token::Kind::LET) {
    get();

    // Read variable name
    if(empty() || peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected variable name");
    }
    Token* variableName = get();

    if(currScopeLevel == 0) {
      return declareGlobal(variableName);
    } else {
      return declareLocal(variableName);
    }
  }

  if(next->type == Token::Kind::IDENTIFIER && next->text == "print") {
    get();
    if(empty() || peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected opening parenthesis");
    }
    get();
    Expression* expression = readExpression();
    if(empty() || peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    get();
    Statement* stmt = new Statement(StatementType::PRINT);
    stmt->print = new Statement::Print;
    stmt->print->expression = expression;

    if(empty() || peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected semicolon");
    }
    get();

    return stmt;
  }

  throw new SyntaxException("Invalid token");
}

std::vector<Statement*> Compiler::readBlock() {
  if(peek()->type != Token::Kind::LBRACE) {
    throw new SyntaxException("Expected { to open block");
  }
  get();

  std::vector<Statement*> statements;
  while(true) {
    if(empty()) {
      throw new SyntaxException("Unexpected end of block");
    }

    if(peek()->type == Token::Kind::RBRACE) {
      get();
      break;
    }
    statements.push_back(readStatement());
  }

  return statements;
}

Expression* Compiler::readExpression(bool fromGroup) {
  // Assignment expression
  if(require(2) && peek()->type == Token::Kind::IDENTIFIER && peek(1)->type == Token::Kind::EQ) {
    Token* name = get();

    int localIndex = -1;
    for(int i=locals.size()-1;i>=0;i--) {
      if(locals[i].name == name->text && locals[i].depth <= currScopeLevel) {
        localIndex = i;
        break;
      }
    }

    // Remove equal sign
    get();

    if(localIndex == -1) {
      // Global variable
      if(!output.globalNames.contains(name->text)) {
        throw new SyntaxException("Use of undeclared variable");
      }
      Expression* value = readExpression();
      Expression* node = new Expression(ExpressionType::SET_GLOBAL);
      node->setGlobal = new Expression::SetGlobal;
      node->setGlobal->name = name->text;
      node->setGlobal->value = value;
      return node;
    } else {
      // Local variable
      Expression* value = readExpression();
      Expression* node = new Expression(ExpressionType::SET_LOCAL);
      node->setLocal = new Expression::SetLocal;
      node->setLocal->name = name->text;
      node->setLocal->value = value;
      node->setLocal->index = localIndex;
      return node;
    }
  }

  // Arithmetic expression
  Expression* term = readTerm();
  if(empty()) {
    return term;
  }

  Token* next = peek();

  if(next->type == Token::Kind::PLUS || next->type == Token::Kind::MINUS) {
    get();
    Expression* result = new Expression(ExpressionType::ARITHMETIC);
    result->arithmetic = new Expression::Arithmetic;
    result->arithmetic->left = term;

    if(fromGroup) {
      if(!empty() && (peek()->type == Token::Kind::IDENTIFIER || peek()->type == Token::Kind::NUMBER || peek()->type == Token::Kind::MINUS || peek()->type == Token::Kind::LPAREN)) {
        result->arithmetic->right = readExpression(true);
      }
    } else {
      result->arithmetic->right = readExpression(false);
    }

    if(next->type == Token::Kind::PLUS) result->arithmetic->type = ArithmeticType::PLUS;
    if(next->type == Token::Kind::MINUS) result->arithmetic->type = ArithmeticType::MINUS;
    
    return result;
  }

  if(next->type == Token::Kind::DEQ) {
    get();
    Expression* result = new Expression(ExpressionType::COMPARISON);
    result->comparison = new Expression::Comparison;
    result->comparison->left = term;
    result->comparison->type = ComparisonType::EQUALS;

    if(fromGroup) {
      if(!empty() && (peek()->type == Token::Kind::IDENTIFIER || peek()->type == Token::Kind::NUMBER || peek()->type == Token::Kind::MINUS || peek()->type == Token::Kind::LPAREN)) {
        result->comparison->right = readExpression(true);
      }
    } else {
      result->comparison->right = readExpression(false);
    }

    return result;
  }

  return term;
}

Expression* Compiler::readTerm() {
  auto factor = readFactor();
  Expression* lhs;

  if(factor.index() == 0) {
    lhs = std::get<Expression*>(factor);
  } else {
    lhs = new Expression(ExpressionType::PRIMARY);
    lhs->primary = std::get<PrimaryExpression*>(factor);
  }

  if(empty()) {
    return lhs;
  }

  Token* next = peek();
  if(next->type == Token::Kind::MULT || next->type == Token::Kind::DIV || next->type == Token::Kind::MOD) {
    get();
    Expression* result = new Expression(ExpressionType::ARITHMETIC);
    result->arithmetic = new Expression::Arithmetic;
    result->arithmetic->left = lhs;
    result->arithmetic->right = readTerm();
    
    if(next->type == Token::Kind::MULT) result->arithmetic->type = ArithmeticType::MULT;
    if(next->type == Token::Kind::DIV) result->arithmetic->type = ArithmeticType::DIV;
    if(next->type == Token::Kind::MOD) result->arithmetic->type = ArithmeticType::MOD;

    return result;
  }

  return lhs;
}

std::variant<Expression*, PrimaryExpression*> Compiler::readFactor() {
  Token* next = peek();

  if(next->type == Token::Kind::MINUS) {
    get();
    Expression* node = new Expression(ExpressionType::NEGATION);
    node->negation = new Expression::Negation;
    node->negation->child = readPrimary();
    return node;
  } else {
    return readPrimary();
  }
}

PrimaryExpression* Compiler::readPrimary() {
  if(empty()) {
    throw new SyntaxException("Expected symbol");
  }

  Token* token = get();
  
  if(token->type == Token::Kind::NUMBER) {
    std::stringstream intstream;
    std::stringstream decimalstream;
    bool decimal = false;

    for(int i=0;i<token->text.length();i++) {
      if(!decimal && isdigit(token->text[i])) {
        intstream << token->text[i];
      } else if(decimal && isdigit(token->text[i])) {
        decimalstream << token->text[i];
      } else if(!decimal && token->text[i] == '.') {
        decimal = true;
      } else {
        throw new SyntaxException("Invalid number");
      }
    }

    std::string intpart;
    std::string decimalpart;
    intstream >> intpart;
    decimalstream >> decimalpart;

    if(decimal) {
      PrimaryExpression* node = new PrimaryExpression(PrimaryType::CONSTANT);
      node->value = NUMBER_VAL(stof(intpart + "." + decimalpart));
      return node;
    } else {
      PrimaryExpression* node = new PrimaryExpression(PrimaryType::CONSTANT);
      node->value = NUMBER_VAL(stof(intpart));
      return node;
    }
  }

  if(token->type == Token::Kind::TRUE || token->type == Token::Kind::FALSE) {
    PrimaryExpression* node = new PrimaryExpression(PrimaryType::CONSTANT);
    node->value = BOOL_VAL(token->type == Token::Kind::TRUE);
    return node;
  }

  if(token->type == Token::Kind::IDENTIFIER) {
    // Check if there is a local with this name
    int localIndex = -1;
    for(int i=locals.size()-1;i>=0;i--) {
      if(locals[i].name == token->text && locals[i].depth <= currScopeLevel) {
        localIndex = i;
        break;
      }
    }

    if(localIndex == -1) {
      // Global variable
      if(!output.globalNames.contains(token->text)) {
        throw new SyntaxException("Use of undeclared variable");
      }
      int idx = output.globalNames[token->text];
      PrimaryExpression* node = new PrimaryExpression(PrimaryType::GLOBAL_VARIABLE);
      node->globalVariableIndex = idx;
      return node;
    } else {
      // Local variable
      PrimaryExpression* node = new PrimaryExpression(PrimaryType::LOCAL_VARIABLE);
      node->localVariableIndex = localIndex;
      return node;
    }
  }

  if(token->type == Token::Kind::STRING_LITERAL) {
    ObjectString* obj = new ObjectString(token->text); // TODO (GC)
    PrimaryExpression* node = new PrimaryExpression(PrimaryType::CONSTANT);
    node->value = OBJECT_VAL(obj);
    return node;
  }

  if(token->type == Token::Kind::LPAREN) {
    PrimaryExpression* node = new PrimaryExpression(PrimaryType::GROUP);
    node->group = readExpression(true);
    Token* next = get();
    if(next->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    return node;
  }

  // Invalid
  throw new SyntaxException("Invalid token");
}

Statement* Compiler::declareGlobal(Token* variableName) {
  if(output.globalNames.contains(variableName->text)) {
    throw new SyntaxException("Cannot redeclare global variable");
  }

  // Pop of equal sign
  if(empty() || peek()->type != Token::Kind::EQ) {
    throw new SyntaxException("Unexpected ending of statement");
  }
  get();

  // Read expression
  Expression* expression = readExpression();
  Statement* stmt = new Statement(StatementType::DECLARE_GLOBAL);
  stmt->declareGlobal = new Statement::SetGlobal;
  stmt->declareGlobal->name = variableName->text;
  stmt->declareGlobal->value = expression;

  if(empty() || peek()->type != Token::Kind::SEMICOLON) {
    throw new SyntaxException("Expected semicolon");
  }
  get();

  return stmt;
}

Statement* Compiler::declareLocal(Token* variableName) {
  // (1) Check if variable name is declared in this scope or any previous scopes
  for(const Local& local : locals) {
    if(local.depth == currScopeLevel && local.name == variableName->text) {
      throw new SyntaxException("This local variable has already been declared.");
    }
  }

  // (2) Register local variable
  locals.push_back(Local{
    .name = variableName->text,
    .depth = currScopeLevel,
  });

  // Pop of equal sign
  if(empty() || peek()->type != Token::Kind::EQ) {
    throw new SyntaxException("Unexpected ending of statement");
  }
  get();

  // Read expression
  Expression* expression = readExpression();
  Statement* stmt = new Statement(StatementType::DECLARE_LOCAL);
  stmt->declareLocal = new Statement::SetLocal;
  stmt->declareLocal->name = variableName->text;
  stmt->declareLocal->value = expression;
  stmt->declareLocal->index = locals.size() - 1;

  // Read semicolon
  if(empty() || peek()->type != Token::Kind::SEMICOLON) {
    throw new SyntaxException("Expected semicolon");
  }
  get();
  
  return stmt;
}

} 