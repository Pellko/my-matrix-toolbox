#ifndef _MY_MATRIX_TOOLBOX_COMPILER_LEXER_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_LEXER_H_

#include <string>
#include <ctype.h>
#include <vector>
#include <sstream>
#include "LexicalException.hh"

namespace mymatrixtoolbox {

class Token {
 public:
  enum class Kind {
    NUMBER,
    IDENTIFIER,
    LPAREN,
    RPAREN,
    SEMICOLON,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    LBRACE,
    RBRACE,
    IF,
    ELIF,
    ELSE,
    DEQ,
    EQ,
    FN,
    STRING_LITERAL,
    LET,
    TRUE,
    FALSE,
    FOR,
    LT,
    LEQ,
    GT,
    GEQ,
    WHILE,
    PLUSPLUS,
    MINUSMINUS,
    QUESTION,
    COLON,
    RETURN,
    COMMA,
    AT,
    DOUBLE_ARROW,
    LBRACKET,
    RBRACKET,
    BACKSLASH,
    DBACKSLASH,
  };

  Kind type;
  std::string text;
  int startOffset;
  int endOffset;
  int lineNumber;
};

class Lexer {
 public:
  Lexer(std::string& code);
  void lex(std::vector<Token>& tokens);

 private:
  std::string& code;
  int position = 0;
  int lineNumber = 0;
  char peek(int n=0);
  char get();
  bool empty();

  void pushTrivialToken(Token::Kind type, std::vector<Token>& tokens) {
    tokens.push_back(Token{
      .type = type,
      .text = std::string(1, peek()),
      .startOffset = position,
      .endOffset = position,
      .lineNumber = lineNumber
    });
    get();
  }
  std::string substr(std::string& str, int start, int end) {
    return str.substr(start, end - start);
  }

  int remaining() {
    return code.length() - position;
  }

  void readNumber(std::vector<Token>& tokens);
  void readIdentifierOrKeyword(std::vector<Token>& tokens);
  void readDivOrComment(std::vector<Token>& tokens);
  void readEqOrDeqOrDoubleArrow(std::vector<Token>& tokens);
  void readStringLiteral(std::vector<Token>& tokens);
  void readLtOrLeq(std::vector<Token>& tokens);
  void readGtOrGeq(std::vector<Token>& tokens);
  void readPlusOrDoublePlus(std::vector<Token>& tokens);
  void readMinusOrDoubleMinus(std::vector<Token>& tokens);
  void readBackslashOrDoubleBackslash(std::vector<Token>& tokens);
};

}

#endif