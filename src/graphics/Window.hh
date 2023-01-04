#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_

#include <string>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

namespace mymatrixtoolbox {

class Window {
 public:
  Window(std::string title);
  ~Window();

  void init();
  void terminate();
  bool shouldClose();

  void begin();
  void end();

  int getWidth();
  int getHeight();

 private:
  std::string title;
  bgfx::ViewId kClearView = 0;
  GLFWwindow* window;
  int width;
  int height;
};

}

#endif