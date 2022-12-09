#ifndef _SCISCRIPT_COMPILER_SYNTAX_EXCEPTION_H_
#define _SCISCRIPT_COMPILER_SYNTAX_EXCEPTION_H_

#include <exception>
#include <iostream>

namespace sciscript {

class SyntaxException : public std::exception {
 public:
  SyntaxException(std::string error) {}
  ~SyntaxException() {}

  virtual const char* what() const noexcept {
    return "Syntax Exception";
  }
};

}

#endif