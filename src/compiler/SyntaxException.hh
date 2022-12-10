#ifndef _SCISCRIPT_COMPILER_SYNTAX_EXCEPTION_H_
#define _SCISCRIPT_COMPILER_SYNTAX_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <stdexcept>

namespace sciscript {

class SyntaxException : public std::exception {
 public:
  SyntaxException(std::string error) : error(error) {}
  ~SyntaxException() {}
  std::string error;

  virtual const char* what() const throw() {
    return error.c_str();
  }
};

}

#endif