#ifndef _SCISCRIPT_COMPILER_LEXICAL_EXCEPTION_H_
#define _SCISCRIPT_COMPILER_LEXICAL_EXCEPTION_H_

#include <exception>
#include <iostream>

namespace sciscript {

class LexicalException : public std::exception {
 public:
  LexicalException() {}
  ~LexicalException() {}

  virtual const char* what() const noexcept {
    return "Lexical Exception";
  }
};

}

#endif