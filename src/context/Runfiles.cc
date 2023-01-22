#include "Runfiles.hh"
#include "whereami.h"

namespace mymatrixtoolbox {

std::string Runfiles::getRoot() {
  int length = wai_getModulePath(NULL, 0, NULL);
  int dirname_length = 0;
  char* path = (char*) malloc(length + 1);
  wai_getModulePath(path, length, &dirname_length);
  path[length] = '\0';
  std::string result(path);
  free(path);
  result.resize(dirname_length);
  return result;
}

std::string Runfiles::convert(std::string path) {
  return getRoot() + "/mymatrixtoolbox.runfiles/__main__/" + path;
}
  
}