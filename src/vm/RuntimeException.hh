#ifndef _SCISCRIPT_VM_RUNTIME_EXCEPTION_H_
#define _SCISCRIPT_VM_RUNTIME_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <stdexcept>

namespace sciscript {

class RuntimeException : public std::exception {
 public:
  RuntimeException(std::string error) : error(error) {}
  ~RuntimeException() {}
  std::string error;

  virtual const char* what() const throw() {
    return error.c_str();
  }
};

}

#endif