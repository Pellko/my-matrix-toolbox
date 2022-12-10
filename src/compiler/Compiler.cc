#include "Compiler.hh"
// #include "src/compiler/Expression.hh"
// #include "src/compiler/Lexer.hh"
// #include "src/compiler/Statement.hh"
// #include "src/compiler/SyntaxException.hh"
// #include "src/vm/VirtualMachine.hh"
// #include <iostream>
// #include <variant>

namespace sciscript {

Compiler::Compiler(std::vector<Token>& tokens) : tokens(tokens) {}

CompilerOutput Compiler::compile() {
  position = 0;
  CompilerOutput output;

  statement(output);

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

  if(next->type == Token::Kind::LET) {
    get();

    // Read variable name
    if(empty() || peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected variable name");
    }
    Token* variableName = get();

    // Pop of equal sign
    if(empty() || peek()->type != Token::Kind::EQ) {
      throw new SyntaxException("Unexpected ending of statement");
    }
    get();

    // Read expression
    Expression* expression = readExpression();
    Statement* stmt = new Statement(StatementType::DECLARE_GLOBAL);
    stmt->declareGlobal = new Statement::DeclareGlobal;
    stmt->declareGlobal->name = variableName->text;
    stmt->declareGlobal->initializer = expression;

    if(empty() || peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected semicolon");
    }

    return stmt;
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

    return stmt;
  }

  throw new SyntaxException("Invalid token");
}

Expression* Compiler::readExpression(bool fromGroup) {
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

}