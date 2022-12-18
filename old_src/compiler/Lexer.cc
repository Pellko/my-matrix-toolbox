#include "Lexer.hh"

namespace sciscript {

Lexer::Lexer(std::string& code) : code(code) {}

bool Lexer::empty() {
  return position == code.length();
}

char Lexer::peek(int n) {
  if(position == code.length()) {
    return '\0';
  } else {
    return code.c_str()[position + n];
  }
}

char Lexer::get() {
  char currChar = code.c_str()[position];
  position++;
  return currChar;
}

void Lexer::lex(std::vector<Token>& tokens) {
  position = 0;
  lineNumber = 1;

  while(!empty()) {
    char currChar = peek();

    if(currChar == '\n') {
      lineNumber++;
    }

    if(isdigit(currChar)) {
      readNumber(tokens);
    } else if(isalpha(currChar) || currChar == '_' || currChar == '$') {
      readIdentifierOrKeyword(tokens);
    } else if(currChar == '(') {
      pushTrivialToken(Token::Kind::LPAREN, tokens);
    } else if(currChar == ')') {
      pushTrivialToken(Token::Kind::RPAREN, tokens);
    } else if(currChar == '+') {
      readPlusOrDoublePlus(tokens);
    } else if(currChar == '-') {
      readMinusOrDoubleMinus(tokens); 
    } else if(currChar == '*') {
      pushTrivialToken(Token::Kind::MULT, tokens);
    } else if(currChar == '/') {
      readDivOrComment(tokens);
    } else if(currChar == '%') {
      pushTrivialToken(Token::Kind::MOD, tokens);
    } else if(currChar == ';') {
      pushTrivialToken(Token::Kind::SEMICOLON, tokens);
    } else if(currChar == '{') {
      pushTrivialToken(Token::Kind::LBRACE, tokens);
    } else if(currChar == '}') {
      pushTrivialToken(Token::Kind::RBRACE, tokens);
    } else if(currChar == '?') {
      pushTrivialToken(Token::Kind::QUESTION, tokens);
    } else if(currChar == ':') {
      pushTrivialToken(Token::Kind::COLON, tokens);
    } else if(currChar == '<') {
      readLtOrLeq(tokens);
    } else if(currChar == '>') {
      readGtOrGeq(tokens);
    } else if(currChar == '"') {
      readStringLiteral(tokens);
    } else if(currChar == '=') {
      readEqOrDeq(tokens);
    } else if (currChar == ' ' || currChar == '\n' || currChar == '\t') {
      get();
    } else {
      throw new LexicalException();
    }
  }
}

void Lexer::readNumber(std::vector<Token>& tokens) {
  int startOffset = position;
  bool hasDecimalPoint = false;

  while(true) {
    if(isdigit(peek())) {
      get();
    } else if(peek() == '.' && !hasDecimalPoint) {
      hasDecimalPoint = true;
      get();
    } else {
      break;
    }
  }

  tokens.push_back(Token{
    .type = Token::Kind::NUMBER,
    .text = substr(code, startOffset, position),
    .startOffset = startOffset,
    .endOffset = position - 1,
    .lineNumber = lineNumber,
  });
}

void Lexer::readIdentifierOrKeyword(std::vector<Token>& tokens) {
  int startOffset = position;
  std::stringstream ss;
  std::string id;
  ss << get();

  while(isalpha(peek()) || isdigit(peek()) || peek() == '_' || peek() == '$') {
    ss << get();
  }
  ss >> id;

  // Check if we have a keyword
  Token::Kind type;

  if(id == "if") {
    type = Token::Kind::IF;
  } else if(id == "elif") {
    type = Token::Kind::ELIF;
  } else if(id == "else") {
    type = Token::Kind::ELSE;
  } else if(id == "fn") {
    type = Token::Kind::FN;
  } else if(id == "let") {
    type = Token::Kind::LET;
  } else if(id == "true") {
    type = Token::Kind::TRUE;
  } else if(id == "false") {
    type = Token::Kind::FALSE;
  } else if(id == "for") {
    type = Token::Kind::FOR;
  } else if(id == "while") {
    type = Token::Kind::WHILE;
  } else {
    type = Token::Kind::IDENTIFIER;
  }

  tokens.push_back(Token{
    .type = type,
    .text = substr(code, startOffset, position),
    .startOffset = startOffset,
    .endOffset = position,
    .lineNumber = lineNumber,
  });
}

void Lexer::readDivOrComment(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '/') {
    get();
    get();
    while(peek() != '\n') {
      get();
    }
    get();
  } else if(remaining() >= 2 && peek(1) == '*') {
    get();
    get();
    while(remaining() >= 2 && !(peek() == '*' && peek(1) == '/')) {
      get();
    }
    get();
    get();
  } else {
    pushTrivialToken(Token::Kind::DIV, tokens);
  }
}

void Lexer::readEqOrDeq(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '=') {
    int startOffset = position;
    get();
    get();
    tokens.push_back(Token{
      .type = Token::Kind::DEQ,
      .text = substr(code, startOffset, position),
      .startOffset = startOffset,
      .endOffset = position,
      .lineNumber = lineNumber
    });
  } else {
    pushTrivialToken(Token::Kind::EQ, tokens);
  }
}

void Lexer::readStringLiteral(std::vector<Token>& tokens) {
  get();
  int startOffset = position;
  std::stringstream ss;
  std::string literal;
  
  while(true) {
    if(empty()) {
      throw new LexicalException();
    }

    if(!empty() && peek() == '"') {
      get();
      break;
    }

    ss << get();
  }
  ss >> literal;
  tokens.push_back(Token{
    .type = Token::Kind::STRING_LITERAL,
    .text = substr(code, startOffset, position-1),
    .startOffset = startOffset,
    .endOffset = position - 1,
    .lineNumber = lineNumber,
  });
}

void Lexer::readLtOrLeq(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '=') {
    int startOffset = position;
    get();
    get();
    tokens.push_back(Token{
      .type = Token::Kind::LEQ,
      .text = substr(code, startOffset, position),
      .startOffset = startOffset,
      .endOffset = position,
      .lineNumber = lineNumber
    });
  } else {
    pushTrivialToken(Token::Kind::LT, tokens);
  }
}

void Lexer::readGtOrGeq(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '=') {
    int startOffset = position;
    get();
    get();
    tokens.push_back(Token{
      .type = Token::Kind::GEQ,
      .text = substr(code, startOffset, position),
      .startOffset = startOffset,
      .endOffset = position,
      .lineNumber = lineNumber
    });
  } else {
    pushTrivialToken(Token::Kind::GT, tokens);
  }
}

void Lexer::readPlusOrDoublePlus(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '+') {
    int startOffset = position;
    get();
    get();
    tokens.push_back(Token{
      .type = Token::Kind::PLUSPLUS,
      .text = substr(code, startOffset, position),
      .startOffset = startOffset,
      .endOffset = position,
      .lineNumber = lineNumber
    });
  } else {
    pushTrivialToken(Token::Kind::PLUS, tokens);
  }
}

void Lexer::readMinusOrDoubleMinus(std::vector<Token>& tokens) {
  if(remaining() >= 2 && peek(1) == '-') {
    int startOffset = position;
    get();
    get();
    tokens.push_back(Token{
      .type = Token::Kind::MINUSMINUS,
      .text = substr(code, startOffset, position),
      .startOffset = startOffset,
      .endOffset = position,
      .lineNumber = lineNumber
    });
  } else {
    pushTrivialToken(Token::Kind::MINUS, tokens);
  }
}

}