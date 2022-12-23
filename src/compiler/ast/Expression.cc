#include "Expression.hh"
#include "AssignPropertyExpression.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/ast/AssignVariableExpression.hh"
#include "src/compiler/ast/BinaryExpression.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"
#include "src/compiler/ast/LambdaExpression.hh"
#include "src/compiler/ast/PrimaryExpression.hh"
#include "src/compiler/ast/UnaryExpression.hh"
#include "src/compiler/ast/CallExpression.hh"
#include "src/compiler/ast/FunctionStatement.hh"
#include "src/compiler/ast/BlockStatement.hh"
#include "src/types/Local.hh"

namespace mymatrixtoolbox {

Expression* Expression::parse(ParserTool& parserTool) {
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

  // Lambda function
  if(parserTool.require(1) && parserTool.peek()->type == Token::Kind::AT) {
    parserTool.get();

    parserTool.beginLambda();
    int functionIndex = parserTool.getFunctions().size() - 1;

    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected (");
    }
    parserTool.get();
    std::vector<Argument> argList = FunctionStatement::readArgumentList(parserTool);

    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected )");
    }
    parserTool.get();

    // Read arrow
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::DOUBLE_ARROW) {
      throw new SyntaxException("Expected =>");
    }
    parserTool.get();

    // Read function body
    parserTool.beginScope();

    // Insert arguments as locals
    for(Argument& arg : argList) {
      parserTool.registerLocal(arg.name);
      parserTool.currentScope()->chunk.numArguments++;
    }

    // Read function body
    std::cout << parserTool.peek()->text << std::endl;
    Statement* body = Statement::parse(parserTool);
    std::cout << parserTool.peek()->text << std::endl;
    BlockStatement* block = new BlockStatement();
    block->addStatement(body);
    LambdaExpression* node = new LambdaExpression(body, parserTool.currentScope());
    parserTool.storeLocalsInBlockStatement(block);
    parserTool.endScope();
    parserTool.endFunction();
    node->setFunctionIndex(functionIndex);

    if(parserTool.currentScope() == nullptr) {
      node->setParentScope(nullptr);
    } else {
      node->setParentScope(parserTool.currentScope());
    }

    for(Upvalue u : parserTool.getFunctions()[functionIndex]->upvalues) {
      node->addUpvalue(u);
    }

    node->setArguments(argList);

    return node;
  }

  Expression* expression = readArithmeticExpression(parserTool);

  // Equality comparsion
  if(parserTool.require(1) && parserTool.peek()->type == Token::Kind::DEQ) {
    parserTool.get();
    Expression* rhs = parse(parserTool);
    BinaryExpression* node = new BinaryExpression(BinaryOperation::EQUALITY, expression, rhs);
    return node;
  }

  // Inequality comparsion
  if(
    parserTool.require(1) && (
      parserTool.peek()->type == Token::Kind::LT ||
      parserTool.peek()->type == Token::Kind::LEQ ||
      parserTool.peek()->type == Token::Kind::GT ||
      parserTool.peek()->type == Token::Kind::GEQ
    )
  ) {
    Token* op = parserTool.get();
    Expression* rhs = parse(parserTool);
    BinaryOperation type;
    if(op->type == Token::Kind::LT) type = BinaryOperation::LESS_THAN;
    if(op->type == Token::Kind::LEQ) type = BinaryOperation::LESS_THAN_EQUALS;
    if(op->type == Token::Kind::GT) type = BinaryOperation::GREATER_THAN;
    if(op->type == Token::Kind::GEQ) type = BinaryOperation::GREATER_THAN_EQUALS;

    BinaryExpression* node = new BinaryExpression(type, expression, rhs);
    return node;
  }

  // Dot operator


  return expression;
}

Expression* Expression::readArithmeticExpression(ParserTool& parserTool) {
  if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::RPAREN) {
    return nullptr;
  }

  Expression* term = readTerm(parserTool);
  if(parserTool.empty()) {
    return term;
  }

  Token* next = parserTool.peek();
  if(next->type == Token::Kind::PLUS || next->type == Token::Kind::MINUS) {
    parserTool.get();
    Expression* rhs = parse(parserTool);
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
    Expression* expression = readPrimary(parserTool);

    // Function call
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::LPAREN) {
      return readCall(parserTool, expression);
    }

    // Array index
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::LBRACKET) {
      parserTool.get();

      Expression* row = Expression::parse(parserTool);
      Expression* col;

      if(parserTool.empty()) {
        throw new SyntaxException("Unexpected ending of file");
      }

      if(parserTool.peek()->type == Token::Kind::RBRACKET) {
        col = new ConstantExpression(Literal::fromDouble(0));
        parserTool.get();
      } else {
        if(parserTool.peek()->type != Token::Kind::COMMA) {
          throw new SyntaxException("Expected ,");
        }
        parserTool.get();
        col = Expression::parse(parserTool);
        if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RBRACKET) {
          throw new SyntaxException("Expected ]");
        }
        parserTool.get();
      }
      return new MatrixAccessExpression(expression, row, col);
    }

    // Access property
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::DOT) {
      parserTool.get();

      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::IDENTIFIER) {
        throw new SyntaxException("Expected property name");
      }
      Token* propertyName = parserTool.get();

      // Check if we are assigning or not
      if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::EQ) {
        parserTool.get();
        Expression* value = Expression::parse(parserTool);
        AssignPropertyExpression* node = new AssignPropertyExpression(propertyName->text, expression, value);
        return node;
      } else {
        ReadPropertyExpression* node = new ReadPropertyExpression(propertyName->text, expression);

        // Check if we are calling
        if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::LPAREN) {
          return readCall(parserTool, node);
        } else {
          return node;
        }
      }
    }

    return expression;
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

  if(token->type == Token::Kind::TRUE || token->type == Token::Kind::FALSE) {
    Literal literal = Literal::fromBool(token->type == Token::Kind::TRUE);
    ConstantExpression* node = new ConstantExpression(literal);
    return node;
  }

  if(token->type == Token::Kind::STRING_LITERAL) {
    Literal literal = Literal::fromString(token->text);
    ConstantExpression* node = new ConstantExpression(literal);
    return node;
  }

  if(token->type == Token::Kind::LBRACKET) {
    // Read all expressions
    int currX = 0;
    int currY = 0;
    std::vector<std::vector<Expression*>> values;

    while(true) {
      if(parserTool.empty()) {
        throw new SyntaxException("Unexpected ending of file");
      }

      if(parserTool.peek()->type == Token::Kind::RBRACKET) {
        parserTool.get();
        break;
      }

      if(values.size() <= currX) {
        values.resize(values.size()+1);
      }

      if(values[currX].size() <= currY) {
        values[currX].resize(values[currX].size() + 1);
      }

      values[currX][currY] = Expression::parse(parserTool);
      
      if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::DBACKSLASH) {
        currX = 0;
        currY++;
        parserTool.get();
      } else if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::COMMA) {
        currX++;
        parserTool.get();
      }
    }

    // Find the actual matrix width and height
    int width = values.size();
    int height = 0;
    for(std::vector<Expression*>& col : values) {
      if(col.size() > height) {
        height = col.size();
      }
    }

    MatrixExpression* node = new MatrixExpression(width, height);
    for(int x=0;x<width;x++) {
      for(int y=0;y<height;y++) {
        std::vector<Expression*>& col = values[x];
        if(col.size() <= y) {
          node->addExpression(x, y, new ConstantExpression(Literal::fromDouble(0)));
        } else {
          node->addExpression(x, y, col[y]);
        }
      }
    }

    return node;
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
    Expression* expression = Expression::parse(parserTool);
    GroupExpression* node = new GroupExpression(expression);
    Token* next = parserTool.get();
    if(next->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    return node;
  }

  std::cout << token->text << std::endl;
  throw new SyntaxException("Invalid token");
}

Expression* Expression::readCall(ParserTool& parserTool, Expression* target) {
  if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
    throw new SyntaxException("Expected ( when performing a call");
  }
  parserTool.get();

  // Read argument
  std::vector<Expression*> arguments;

  while(!parserTool.empty() && parserTool.peek()->type != Token::Kind::RPAREN) {
    arguments.push_back(parse(parserTool));

    if(parserTool.empty()) {
      throw new SyntaxException("Unexpected end of argument list in call");
    }

    if(parserTool.peek()->type == Token::Kind::COMMA) {
      parserTool.get();
    }
  }

  if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
    throw new SyntaxException("Expected ) when calling function");
  }
  parserTool.get();
  CallExpression* node = new CallExpression(target, arguments);
  return node;
}

}