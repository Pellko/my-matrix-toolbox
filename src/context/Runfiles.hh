#ifndef _MY_MATRIX_TOOLBOX_CONTEXT_RUNFILES_H_
#define _MY_MATRIX_TOOLBOX_CONTEXT_RUNFILES_H_

#include <string>

namespace mymatrixtoolbox {

class Runfiles {
 public:
  Runfiles() {}
  ~Runfiles() {}

  static std::string getRoot();
  static std::string convert(std::string path);
};

}

#endif