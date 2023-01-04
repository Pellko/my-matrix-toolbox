#include "Statement.hh"
#include "DeclareClassStatement.hh"
#include "src/compiler/Lexer.hh"
#include "src/compiler/SyntaxException.hh"
#include "src/compiler/ast/DeclareVariableStatement.hh"
#include "src/compiler/ast/Expression.hh"
#include "src/compiler/ast/ForStatement.hh"
#include "src/compiler/ast/FunctionStatement.hh"
#include "src/compiler/ast/BlockStatement.hh"
#include "src/compiler/ast/PrintStatement.hh"
#include "src/compiler/ast/IfStatement.hh"
#include "ExpressionStatement.hh"
#include "PrimaryExpression.hh"
#include "src/compiler/ast/ReturnStatement.hh"

namespace mymatrixtoolbox {

Statement* Statement::parse(ParserTool& parserTool) {
  Token* next = parserTool.peek();

  // Block
  if(next->type == Token::Kind::LBRACE) {
    parserTool.beginScope();
    std::vector<Statement*> statements = readBlock(parserTool);
    BlockStatement* node = new BlockStatement();
    for(Statement* stmt : statements) {
      node->addStatement(stmt);
    }
    parserTool.storeLocalsInBlockStatement(node);
    parserTool.endScope();
    return node;
  }

  // Function
  if(next->type == Token::Kind::FN) {
    parserTool.get();
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected function name");
    }
    Token* name = parserTool.get();
    // TODO: Ensure that this doesnt redeclare function
    parserTool.beginFunction(name->text);
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
    
    // Read function body
    parserTool.beginScope();

    // Insert arguments as locals
    for(Argument& arg : argList) {
      parserTool.registerLocal(arg.name);
      parserTool.currentScope()->chunk.numArguments++;
    }    
    
    bool isGlobal = parserTool.getScopeLevel() == 1;
    int globalIndex = -1;
    if(isGlobal) {
      globalIndex = parserTool.registerGlobal(name->text);
    }

    std::vector<Statement*> statements = readBlock(parserTool);
    BlockStatement* block = new BlockStatement();
    for(Statement* stmt : statements) {
      block->addStatement(stmt);
    }

    FunctionStatement* fn = new FunctionStatement(name->text, isGlobal, block, parserTool.currentScope());
    parserTool.storeLocalsInBlockStatement(block);
    parserTool.endScope();
    parserTool.endFunction();
  
    if(isGlobal) {
      fn->setGlobalIndex(globalIndex);
    } else {
      fn->setGlobalIndex(-1);
    }

    fn->setFunctionIndex(functionIndex);
    if(parserTool.currentScope() == nullptr) {
      fn->setParentScope(nullptr);
    } else {
      fn->setParentScope(parserTool.currentScope());
    }
    
    for(Upvalue u : parserTool.getFunctions()[functionIndex]->upvalues) {
      fn->addUpvalue(u);
    }
    
    fn->setArguments(argList);
    
    return fn;
  }

  // Class declaration
  if(next->type == Token::Kind::CLASS) {
    parserTool.get();

    // Read class name
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected class name");
    }
    Token* name = parserTool.get();

    // Make sure that class name is available
    int classIndex = -1;
    bool isGlobal = parserTool.currentScope() == 0;

    if(parserTool.currentScope() == 0) {
      auto [_, index] = parserTool.findIdentifier(name->text);
      if(index != -1) {
        throw new SyntaxException("You cannot declare a class");
      }
      classIndex = parserTool.registerGlobal(name->text);
    } else {
      // Check that class name hasnt been declared already
      for(const Local& local : parserTool.getLocals()) {
        if(local.depth == parserTool.getScopeLevel() && local.name == name->text) {
          throw new SyntaxException("This class name has already been declared");
        }
      }

      classIndex = parserTool.registerLocal(name->text);
    }

    // Pop of opening bracket
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LBRACE) {
      throw new SyntaxException("Expected { after class identifier");
    }
    parserTool.get();

    // Open a class scope
    parserTool.beginScope();

    // Method declarations
    DeclareClassStatement* node = new DeclareClassStatement(classIndex, isGlobal);

    while(true) {
      if(parserTool.empty()) {
        throw new SyntaxException("Unexpected ending of class declaration");
      }

      if(parserTool.peek()->type == Token::Kind::RBRACE) {
        parserTool.get();
        break;
      }

      // Read methods name
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::IDENTIFIER) {
        throw new SyntaxException("Expected method name");
      }
      Token* methodName = parserTool.get();

      // Read arguments
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
        throw new SyntaxException("Expected ( after method name");
      }
      parserTool.get();
      std::vector<Argument> argList = FunctionStatement::readArgumentList(parserTool);
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
        throw new SyntaxException("Expected ) after method argument list");
      }
      parserTool.get();

      parserTool.beginFunction(methodName->text);
      int functionIndex = parserTool.getFunctions().size() - 1;

      // Begin method scope
      parserTool.beginScope();

      // Insert this as local
      parserTool.registerLocal("this");

      // Insert arguments as locals
      for(Argument& arg : argList) {
        parserTool.registerLocal(arg.name);
        parserTool.currentScope()->chunk.numArguments++;
      }

      // Read method body
      CompilerScope* methodScope = parserTool.currentScope();
      std::vector<Statement*> statements = readBlock(parserTool);
      BlockStatement* block = new BlockStatement();
      for(Statement* stmt : statements) {
        block->addStatement(stmt);
      }
      parserTool.storeLocalsInBlockStatement(block);
      parserTool.endScope();
      parserTool.endFunction();

      node->addMethod(ClassMethod{
        .name = methodName->text,
        .arguments = argList,
        .block = block,
        .compilerScope = methodScope,
        .upvalues = parserTool.getFunctions()[functionIndex]->upvalues,
        .functionIndex = functionIndex,
      });
    }

    // End class scope
    parserTool.endScope();

    return node;    
  }

  // For loop
  if(next->type == Token::Kind::FOR) {
    parserTool.get();

    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected ( in for loop");
    }
    parserTool.get();

    parserTool.beginScope();
    Statement* initializer = nullptr;
    Expression* condition = nullptr;
    Expression* incrementor = nullptr;

    // (1) Read initializer clause
    if(parserTool.peek()->type == Token::Kind::SEMICOLON) {
      parserTool.get();
    } else if(parserTool.peek()->type == Token::Kind::LET) {
      initializer = parse(parserTool);
    } else {
      initializer = new ExpressionStatement(Expression::parse(parserTool));
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::SEMICOLON) {
        throw new SyntaxException("Expected semicolon");
      }
      parserTool.get();
    }

    // (2) Read condition clause
    if(parserTool.peek()->type == Token::Kind::SEMICOLON) {
      parserTool.get();
    } else {
      condition = Expression::parse(parserTool);
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::SEMICOLON) {
        throw new SyntaxException("Expected semicolon");
      }
      parserTool.get();
    }

    // (3) Read incrementor
    if(parserTool.peek()->type == Token::Kind::SEMICOLON) {
      parserTool.get();
    } else {
      incrementor = Expression::parse(parserTool);
    }

    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected )");
    }
    parserTool.get();

    if(parserTool.empty()) {
      throw new SyntaxException("Expected body");
    }

    Statement* body = Statement::parse(parserTool);
    ForStatement* node = new ForStatement();
    node->setInitializer(initializer);
    node->setCondition(condition);
    node->setIncrementor(incrementor);
    node->setBody(body);
    parserTool.endScope();

    return node;
  }

  // If statement
  if(next->type == Token::Kind::IF) {
    parserTool.get();
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected ( after if statement");
    }
    parserTool.get();
    Expression* condition = Expression::parse(parserTool);
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected ) after if statement");
    }
    parserTool.get();
    Statement* trueStatement = Statement::parse(parserTool);
    IfStatement* node = new IfStatement(condition, trueStatement);

    // Read else if statements
    std::vector<std::pair<Expression*, Statement*>> elifStatements;
    while(true) {
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::ELIF) {
        break;
      }
      parserTool.get();

      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
        throw new SyntaxException("Expected ( after elif statement");
      }
      parserTool.get();
      Expression* condition = Expression::parse(parserTool);

      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
        throw new SyntaxException("Expected ) after elif condition");
      }
      parserTool.get();

      Statement* statement = Statement::parse(parserTool);
      node->addElifStatement(condition, statement);
    }

    // Read else statement
    if(!parserTool.empty() && parserTool.peek()->type == Token::Kind::ELSE) {
      parserTool.get();
      node->setElseStatement(Statement::parse(parserTool));
    }

    return node;
  }

  // Variable declaration
  if(next->type == Token::Kind::LET) {
    parserTool.get();
    
    // Read variable name
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::IDENTIFIER) {
      throw new SyntaxException("Expected variable name");
    }
    Token* name = parserTool.get();

    if(parserTool.empty()) {
      throw new SyntaxException("Unexpected end of file");
    }

    // Read variable value
    Expression* value;
    if(parserTool.peek()->type == Token::Kind::SEMICOLON) {
      parserTool.get();
      value = new ConstantExpression(Literal::nil());
    } else if(parserTool.peek()->type == Token::Kind::EQ) {
      parserTool.get();
      value = Expression::parse(parserTool);
      if(parserTool.empty() || parserTool.peek()->type != Token::Kind::SEMICOLON) {
        throw new SyntaxException("Expected ; to end statement");
      }
      parserTool.get();
    } else {
      throw new SyntaxException("Unexpected token");
    }

    if(parserTool.getScopeLevel() == 0) {
      // Check that name is globally unique
      auto [_, index] = parserTool.findIdentifier(name->text);
      if(index != -1) {
        throw new SyntaxException("You cannot redeclare variable");
      }
  
      int newIndex = parserTool.registerGlobal(name->text);
      DeclareVariableStatement* node = new DeclareVariableStatement(DeclareVariableType::GLOBAL, newIndex, value);
      return node;
    } else {
      // Check that variable name hasnt been declared already in this scope
      for(const Local& local : parserTool.getLocals()) {
        if(local.depth == parserTool.getScopeLevel() && local.name == name->text) {
          throw new SyntaxException("This local variable has already been declared");
        }
      }

      // Register local
      int newIndex = parserTool.registerLocal(name->text);
      DeclareVariableStatement* node = new DeclareVariableStatement(DeclareVariableType::LOCAL, newIndex, value);
      return node;
    }
  }

  // Return statement
  if(next->type == Token::Kind::RETURN) {
    parserTool.get();
    if(parserTool.empty()) {
      throw new SyntaxException("Unexpected ending of block");
    }

    if(parserTool.peek()->type == Token::Kind::SEMICOLON) {
      parserTool.get();
      ReturnStatement* node = new ReturnStatement();
      return node;
    }
    Expression* expression = Expression::parse(parserTool);
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected ; to end statement");
    }
    parserTool.get();
    ReturnStatement* node = new ReturnStatement();
    node->setExpression(expression);
    return node;
  }

  // Print statement
  if(next->type == Token::Kind::IDENTIFIER && next->text == "print") {
    parserTool.get();
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::LPAREN) {
      throw new SyntaxException("Expected opening parenthesis");
    }
    parserTool.get();

    Expression* expression = Expression::parse(parserTool);

    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::RPAREN) {
      throw new SyntaxException("Expected closing parenthesis");
    }
    parserTool.get();
    PrintStatement* node = new PrintStatement(expression);
    if(parserTool.empty() || parserTool.peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected semicolon");
    }
    parserTool.get();

    return node;
  }

  // Expression
  if(parserTool.expressionNext()) {
    Expression* expression = Expression::parse(parserTool);
    ExpressionStatement* node = new ExpressionStatement(expression);
    
    if(parserTool.peek()->type != Token::Kind::SEMICOLON) {
      throw new SyntaxException("Expected ; to end statement");
    }
    parserTool.get();
    
    return node;
  }

  throw new SyntaxException("Invalid token");
}

std::vector<Statement*> Statement::readBlock(ParserTool& parserTool) {
  if(parserTool.peek()->type != Token::Kind::LBRACE) {
    throw new SyntaxException("Expected { to open block");
  }
  parserTool.get();

  std::vector<Statement*> statements;

  while(true) {
    if(parserTool.empty()) {
      throw new SyntaxException("Unexpected end of block");
    }

    if(parserTool.peek()->type == Token::Kind::RBRACE) {
      parserTool.get();
      break;
    }
    statements.push_back(parse(parserTool));
  }

  return statements;
}

}