#include "Expression.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/ast/AssignVariableExpression.hh"
#include "src/compiler/ast/BinaryExpression.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"
#include "src/compiler/ast/PrimaryExpression.hh"
#include "src/compiler/ast/UnaryExpression.hh"
#include "src/compiler/ast/CallExpression.hh"
#include "src/types/Local.hh"

namespace sciscript {

Expression* Expression::parse(ParserTool& parserTool, bool fromGroup) {
  // Assignment
  if(parserTool.require(2) && parserTool.peek()->type == Token::Kind::IDENTIFIER && parserTool.peek(1)->type == Token::Kind::EQ) {
    Token* name = parserTool.get();
    parserTool.get(); // equals sign
    Expression* value = Expression::parse(parserTool);

    // Find identifier
    auto [type, index] = parserTool.findIdentifier(name->text);

    switch(type) {
      case VarRefType::GLOBAL: {
        AssignVariableExpression* node = new AssignVariableExpression(DeclareVariableType::GLOBAL, index, false, value);
        return node;
      }
      case VarRefType::LOCAL: {
        AssignVariableExpression* node = new AssignVariableExpression(DeclareVariableType::LOCAL, index, false, value);
        return node;
      }
      case VarRefType::UPVALUE: {
        int upvalueIndex = parserTool.registerUpvalue(name->text);
        AssignVariableExpression* node = new AssignVariableExpression(DeclareVariableType::LOCAL, upvalueIndex, true, value);
        return node;
      }
    }
  }

  Expression* term = readTerm(parserTool);
  if(parserTool.empty()) {
    return term;
  }

  Token* next = parserTool.peek();

  if(next->type == Token::Kind::PLUS || next->type == Token::Kind::MINUS) {
    parserTool.get();
    Expression* rhs;
    if(fromGroup) {
      if(!parserTool.empty() && (parserTool.peek()->type == Token::Kind::IDENTIFIER || parserTool.peek()->type == Token::Kind::NUMBER || parserTool.peek()->type == Token::Kind::MINUS || parserTool.peek()->type == Token::Kind::LPAREN)) {
        rhs = parse(parserTool, true);
      }
    } else {
      rhs = parse(parserTool, false);
    }

    BinaryOperation op;
    switch(next->type) {
      case Token::Kind::PLUS:
        op = BinaryOperation::ADDITION;
        break;
      case Token::Kind::MINUS:
        op = BinaryOperation::SUBTRACTION;
        break;
      default:
        throw new SyntaxException("Unexpected token in expression");
    }

    BinaryExpression* node = new BinaryExpression(op, term, rhs);
    return node;
  }

  // Calling value
  if(next->type == Token::Kind::LPAREN) {
    parserTool.get();
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected ) when calling function");
    }
    parserTool.get();
    CallExpression* node = new CallExpression(term);
    return node;
  }

  return term;
}

Expression* Expression::readTerm(ParserTool& parserTool) {
  Expression* lhs = readFactor(parserTool);
  if(parserTool.empty()) {
    return lhs;
  }

  Token* next = parserTool.peek();
  if(next->type == Token::Kind::MULT || next->type == Token::Kind::DIV || next->type == Token::Kind::MOD) {
    parserTool.get();
    Expression* rhs = readTerm(parserTool);
    BinaryOperation op;
    switch(next->type) {
      case Token::Kind::MULT:
        op = BinaryOperation::MULTIPLICATION;
        break;
      case Token::Kind::DIV:
        op = BinaryOperation::DIVISION;
        break;
      case Token::Kind::MOD:
        op = BinaryOperation::MODULUS;
        break;
      default:
        throw new SyntaxException("Unexpected token in term");
    }

    BinaryExpression* node = new BinaryExpression(op, lhs, rhs);
    return node;
  }

  return lhs;
}

Expression* Expression::readFactor(ParserTool& parserTool) {
  Token* next = parserTool.peek();

  if(next->type == Token::Kind::MINUS) {
    parserTool.get();
    Expression* expression = readPrimary(parserTool);
    UnaryExpression* node = new UnaryExpression(UnaryOperation::NEGATION, expression);
    return node;
  } else {
    return readPrimary(parserTool);
  }
}

Expression* Expression::readPrimary(ParserTool& parserTool) {
  if(parserTool.empty()) {
    throw new SyntaxException("Expected symbol");
  }

  Token* token = parserTool.get();

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
      Literal literal = Literal::fromDouble(stof(intpart + "." + decimalpart));
      ConstantExpression* node = new ConstantExpression(literal);
      return node;
    } else {
      Literal literal = Literal::fromDouble(stof(intpart));
      ConstantExpression* node = new ConstantExpression(literal);
      return node;
    }
  }

  if(token->type == Token::Kind::IDENTIFIER) {
    auto [type, index] = parserTool.findIdentifier(token->text);

    if(index == -1) {
      throw new SyntaxException("Undeclared identifier");
    }

    switch(type) {
      case VarRefType::GLOBAL: {
        LocalExpression* expression = new LocalExpression(VarRefType::GLOBAL, index);
        return expression;
      }
      case VarRefType::LOCAL: {
        LocalExpression* expression = new LocalExpression(VarRefType::LOCAL, index);
        return expression;
      }
      case VarRefType::UPVALUE: {
        // Register upvalue
        int index = parserTool.registerUpvalue(token->text);
        LocalExpression* expression = new LocalExpression(VarRefType::UPVALUE, index);
        return expression;
      }
    }
  }

  if(token->type == Token::Kind::LPAREN) {
    Expression* expression = Expression::parse(parserTool, true);
    GroupExpression* node = new GroupExpression(expression);
    Token* next = parserTool.get();
    if(next->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    return node;
  }

  throw new SyntaxException("Invalid token");
}

}