#ifndef _MY_MATRIX_TOOLBOX_VM_RUNTIME_EXCEPTION_H_
#define _MY_MATRIX_TOOLBOX_VM_RUNTIME_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <stdexcept>

namespace mymatrixtoolbox {

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