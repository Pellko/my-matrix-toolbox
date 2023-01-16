#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_RENDERABLE_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_RENDERABLE_H_

#include <iostream>

namespace mymatrixtoolbox {

class Renderable {
 public:
  Renderable(int zIndex) : zIndex(zIndex) {}
  virtual ~Renderable() {}

  virtual void init() = 0;
  virtual void render() = 0;
  
  int getZIndex() {
    return zIndex;
  }

 private:
  int zIndex;
};

}

#endif