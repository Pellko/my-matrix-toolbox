#include "Expression.hh"
#include "AssignMatrixExpression.hh"
#include "AssignPropertyExpression.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/ast/AssignVariableExpression.hh"
#include "src/compiler/ast/BinaryExpression.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"
#include "src/compiler/ast/LambdaExpression.hh"
#include "src/compiler/ast/PrimaryExpression.hh"
#include "src/compiler/ast/UnaryExpression.hh"
#include "src/compiler/ast/IncrementExpression.hh"
#include "src/compiler/ast/CallExpression.hh"
#include "src/compiler/ast/FunctionStatement.hh"
#include "src/compiler/ast/BlockStatement.hh"
#include "src/types/Local.hh"
#include <memory>

namespace mymatrixtoolbox {

std::shared_ptr<Expression> Expression::parse(ParserTool& parserTool) {
  // Assignment
  if(parserTool.require(2) && parserTool.peek()->type == Token::Kind::IDENTIFIER && parserTool.peek(1)->type == Token::Kind::EQ) {
    Token* name = parserTool.get();
    parserTool.get(); // equals sign
    std::shared_ptr<Expression> value = Expression::parse(parserTool);
    return readAssignment(parserTool, name->text, [&](std::shared_ptr<Expression> currentValue) {
      return value;
    });
  }

  // Incrementor
  if(
    parserTool.require(2) && 
    parserTool.peek()->type == Token::Kind::IDENTIFIER && 
    (parserTool.peek(1)->type == Token::Kind::PLUSPLUS || parserTool.peek(1)->type == Token::Kind::MINUSMINUS)
  ) {
    Token* name = parserTool.get();
    Token* op = parserTool.get();

    return readAssignment(parserTool, name->text, [&](std::shared_ptr<Expression> currentValue) {
      std::shared_ptr<IncrementExpression> increment = std::make_shared<IncrementExpression>(op->type == Token::Kind::PLUSPLUS, currentValue);
      return increment;
    });
  }

  // Increment by expression
  if(
    parserTool.require(2) &&
    parserTool.peek()->type == Token::Kind::IDENTIFIER &&
    (parserTool.peek(1)->type == Token::Kind::PLUS_EQUALS || parserTool.peek(1)->type == Token::Kind::MINUS_EQUALS)
  ) {
    Token* name = parserTool.get();
    Token* op = parserTool.get();
    std::shared_ptr<Expression> newValue = Expression::parse(parserTool);

    return readAssignment(parserTool, name->text, [&](std::shared_ptr<Expression> currentValue) {
      std::shared_ptr<IncrementExpression> increment = std::make_shared<IncrementExpression>(op->type == Token::Kind::PLUSPLUS, currentValue);
      return increment;
    });
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
    std::shared_ptr<Statement> body = Statement::parse(parserTool);
    std::shared_ptr<BlockStatement> block = std::make_shared<BlockStatement>();
    block->addStatement(body);
    std::shared_ptr<LambdaExpression> node = std::make_shared<LambdaExpression>(body, parserTool.currentScope());
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

  std::shared_ptr<Expression> expression = readArithmeticExpression(parserTool);

  // Equality comparsion
  if(parserTool.require(1) && parserTool.peek()->type == Token::Kind::DEQ) {
    parserTool.get();
    std::shared_ptr<Expression> rhs = parse(parserTool);
    std::shared_ptr<BinaryExpression> node = std::make_shared<BinaryExpression>(BinaryOperation::EQUALITY, expression, rhs);
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
    std::shared_ptr<Expression> rhs = parse(parserTool);
    BinaryOperation type;
    if(op->type == Token::Kind::LT) type = BinaryOperation::LESS_THAN;
    if(op->type == Token::Kind::LEQ) type = BinaryOperation::LESS_THAN_EQUALS;
    if(op->type == Token::Kind::GT) type = BinaryOperation::GREATER_THAN;
    if(op->type == Token::Kind::GEQ) type = BinaryOperation::GREATER_THAN_EQUALS;

    std::shared_ptr<BinaryExpression> node = std::make_shared<BinaryExpression>(type, expression, rhs);
    return node;
  }

  return expression;
}

std::shared_ptr<Expression> Expression::readArithmeticExpression(ParserTool& parserTool) {
  if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::RPAREN) {
    return nullptr;
  }

  std::shared_ptr<Expression> term = readTerm(parserTool);
  if(parserTool.empty()) {
    return term;
  }

  Token* next = parserTool.peek();
  if(next->type == Token::Kind::PLUS || next->type == Token::Kind::MINUS) {
    parserTool.get();
    std::shared_ptr<Expression> rhs = parse(parserTool);
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

    std::shared_ptr<BinaryExpression> node = std::make_shared<BinaryExpression>(op, term, rhs);
    return node;
  }
  return term;
}

std::shared_ptr<Expression> Expression::readTerm(ParserTool& parserTool) {
  std::shared_ptr<Expression> lhs = readFactor(parserTool);
  if(parserTool.empty()) {
    return lhs;
  }

  Token* next = parserTool.peek();
  if(next->type == Token::Kind::MULT || next->type == Token::Kind::DIV || next->type == Token::Kind::MOD) {
    parserTool.get();
    std::shared_ptr<Expression> rhs = readTerm(parserTool);
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

    std::shared_ptr<BinaryExpression> node = std::make_shared<BinaryExpression>(op, lhs, rhs);
    return node;
  }

  return lhs;
}

std::shared_ptr<Expression> Expression::readFactor(ParserTool& parserTool) {
  Token* next = parserTool.peek();

  if(next->type == Token::Kind::MINUS) {
    parserTool.get();
    std::shared_ptr<Expression> expression = readPrimary(parserTool);
    std::shared_ptr<UnaryExpression> node = std::make_shared<UnaryExpression>(UnaryOperation::NEGATION, expression);
    return node;
  } else {
    std::shared_ptr<Expression> expression = readPrimary(parserTool);

    // Function call
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::LPAREN) {
      return readCall(parserTool, expression);
    }

    // Array index
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::LBRACKET) {
      parserTool.get();

      std::shared_ptr<Expression> row = Expression::parse(parserTool);
      std::shared_ptr<Expression> col;

      if(parserTool.empty()) {
        throw new SyntaxException("Unexpected ending of file");
      }

      if(parserTool.peek()->type == Token::Kind::RBRACKET) {
        col = std::make_shared<ConstantExpression>(Literal::fromDouble(0));
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

      // Check if we are assigning
      if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::EQ) {
        parserTool.get();
        std::shared_ptr<Expression> value = Expression::parse(parserTool);
        return std::make_shared<AssignMatrixExpression>(expression, value, row, col);
      } else {
        return std::make_shared<MatrixAccessExpression>(expression, row, col);
      }
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
        std::shared_ptr<Expression> value = Expression::parse(parserTool);
        std::shared_ptr<AssignPropertyExpression> node = std::make_shared<AssignPropertyExpression>(propertyName->text, expression, value);
        return node;
      } else {
        std::shared_ptr<ReadPropertyExpression> node = std::make_shared<ReadPropertyExpression>(propertyName->text, expression);

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

std::shared_ptr<Expression> Expression::readPrimary(ParserTool& parserTool) {
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
      std::shared_ptr<ConstantExpression> node = std::make_shared<ConstantExpression>(literal);
      return node;
    } else {
      Literal literal = Literal::fromDouble(stof(intpart));
      std::shared_ptr<ConstantExpression> node = std::make_shared<ConstantExpression>(literal);
      return node;
    }
  }

  if(token->type == Token::Kind::TRUE || token->type == Token::Kind::FALSE) {
    Literal literal = Literal::fromBool(token->type == Token::Kind::TRUE);
    std::shared_ptr<ConstantExpression> node = std::make_shared<ConstantExpression>(literal);
    return node;
  }

  if(token->type == Token::Kind::STRING_LITERAL) {
    Literal literal = Literal::fromString(token->text);
    std::shared_ptr<ConstantExpression> node = std::make_shared<ConstantExpression>(literal);
    return node;
  }

  // Matrix
  if(token->type == Token::Kind::LBRACKET) {
    // Read all expressions
    int currX = 0;
    int currY = 0;
    std::vector<std::vector<std::shared_ptr<Expression>>> values;

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
    for(std::vector<std::shared_ptr<Expression>>& col : values) {
      if(col.size() > height) {
        height = col.size();
      }
    }

    std::shared_ptr<MatrixExpression> node = std::make_shared<MatrixExpression>(width, height);
    for(int x=0;x<width;x++) {
      for(int y=0;y<height;y++) {
        std::vector<std::shared_ptr<Expression>>& col = values[x];
        if(col.size() <= y) {
          node->addExpression(x, y, std::make_shared<ConstantExpression>(Literal::fromDouble(0)));
        } else {
          node->addExpression(x, y, col[y]);
        }
      }
    }

    return node;
  }

  // Map
  if(token->type == Token::Kind::LBRACE) {
    std::shared_ptr<MapExpression> node = std::make_shared<MapExpression>();
    while(true) {
      if(parserTool.empty()) {
        break;
      }
      if(parserTool.peek()->type != Token::Kind::IDENTIFIER && parserTool.peek()->type != Token::Kind::STRING_LITERAL) {
        break;
      }
      Token* name = parserTool.get();
      
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::COLON) {
        throw new SyntaxException("Expected : after property name");
      }
      parserTool.get();
      std::shared_ptr<Expression> value = Expression::parse(parserTool);

      if(parserTool.peek()->type == Token::Kind::COMMA) {
        parserTool.get();
      } else {
        if(parserTool.peek()->type != Token::Kind::RBRACE) {
          throw new SyntaxException("Expected , after property value in map");
        }
      }

      std::shared_ptr<Expression> nameExpression = std::make_shared<ConstantExpression>(Literal::fromString(name->text));    
      node->addValue(nameExpression, value);  
    }
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RBRACE) {
      throw new SyntaxException("Expected } to close map");
    }
    parserTool.get();
    return node;
  }

  if(token->type == Token::Kind::IDENTIFIER) {
    auto [type, index] = parserTool.findIdentifier(token->text);

    if(index == -1) {
      throw new SyntaxException("Undeclared identifier");
    }

    return readIdentifier(parserTool, token->text);
  }

  if(token->type == Token::Kind::LPAREN) {
    std::shared_ptr<Expression> expression = Expression::parse(parserTool);
    std::shared_ptr<GroupExpression> node = std::make_shared<GroupExpression>(expression);
    Token* next = parserTool.get();
    if(next->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    return node;
  }

  throw new SyntaxException("Invalid token");
}

std::shared_ptr<Expression> Expression::readCall(ParserTool& parserTool, std::shared_ptr<Expression> target) {
  if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
    throw new SyntaxException("Expected ( when performing a call");
  }
  parserTool.get();

  // Read argument
  std::vector<std::shared_ptr<Expression>> arguments;

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
  std::shared_ptr<CallExpression> node = std::make_shared<CallExpression>(target, arguments);
  return node;
}

std::shared_ptr<Expression> Expression::readAssignment(ParserTool& parserTool, std::string identifier, std::function<std::shared_ptr<Expression> (std::shared_ptr<Expression> value)> value) {
  auto [type, index] = parserTool.findIdentifier(identifier);
  switch(type) {
    case VarRefType::GLOBAL: {
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::GLOBAL, index);
      std::shared_ptr<AssignVariableExpression> node = std::make_shared<AssignVariableExpression>(DeclareVariableType::GLOBAL, index, false, value(currentValue));
      return node;
    }
    case VarRefType::LOCAL: {
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::LOCAL, index);
      std::shared_ptr<AssignVariableExpression> node = std::make_shared<AssignVariableExpression>(DeclareVariableType::LOCAL, index, false, value(currentValue));
      return node;
    }
    case VarRefType::UPVALUE: {
      int upvalueIndex = parserTool.registerUpvalue(identifier);
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::UPVALUE, upvalueIndex);
      std::shared_ptr<AssignVariableExpression> node = std::make_shared<AssignVariableExpression>(DeclareVariableType::LOCAL, upvalueIndex, true, value(currentValue));
      return node;
    }
  }
}

std::shared_ptr<Expression> Expression::readIdentifier(ParserTool& parserTool, std::string identifier) {
  auto [type, index] = parserTool.findIdentifier(identifier);
  switch(type) {
    case VarRefType::GLOBAL: {
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::GLOBAL, index);
      return currentValue;
    }
    case VarRefType::LOCAL: {
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::LOCAL, index);
      return currentValue;
    }
    case VarRefType::UPVALUE: {
      int upvalueIndex = parserTool.registerUpvalue(identifier);
      std::shared_ptr<LocalExpression> currentValue = std::make_shared<LocalExpression>(VarRefType::UPVALUE, upvalueIndex);
      return currentValue;
    }
  }
}

}