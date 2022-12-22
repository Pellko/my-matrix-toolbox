#ifndef _MY_MATRIX_TOOLBOX_COMPILER_LEXICAL_EXCEPTION_H_
#define _MY_MATRIX_TOOLBOX_COMPILER_LEXICAL_EXCEPTION_H_

#include <exception>
#include <iostream>

namespace mymatrixtoolbox {

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