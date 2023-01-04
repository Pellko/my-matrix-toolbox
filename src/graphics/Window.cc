#include "Window.hh"

namespace mymatrixtoolbox {

static void errorCallback(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

Window::Window(std::string title) : title(title) {}
Window::~Window() {
  terminate();
}

void Window::init() {
	glfwSetErrorCallback(errorCallback);

  if(!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(1024, 768, title.c_str(), NULL, NULL);
  if(!window) {
    exit(EXIT_FAILURE);
  }

  bgfx::renderFrame();
  bgfx::Init init;
  #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
  #elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
  #elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
  #endif
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;

  if(!bgfx::init(init)) {
    exit(EXIT_FAILURE);
  }

  bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
  bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Window::terminate() {
  bgfx::shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::shouldClose() {
  return glfwWindowShouldClose(window);
}

void Window::begin() {
  glfwPollEvents();
  int oldWidth = width;
  int oldHeight = height;
  glfwGetWindowSize(window, &width, &height);
  if(width != oldWidth || height != oldHeight) {
    bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
  }
  bgfx::touch(kClearView);
}

void Window::end() {
  bgfx::frame();
}

int Window::getWidth() {
  return width;
}

int Window::getHeight() {
  return height;
}

}