#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_

#include <string>
#include "vkb/VkBootstrap.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>

namespace mymatrixtoolbox {

class Window {
 public:
  Window(std::string title);
  ~Window();

  void init();
  void run();
  void draw();
  void terminate();

 private:
  int width = 800;
  int height = 600;
  std::string title;
  GLFWwindow* window;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  VkFormat swapchainImageFormat;
  std::vector<VkImage> swapchainImages;
  std::vector<VkImageView> swapchainImageViews;
  VkQueue graphicsQueue;
  uint32_t graphicsQueueFamily;
  VkCommandPool commandPool;
  VkCommandBuffer mainCommandBuffer;
  VkRenderPass renderPass;
  std::vector<VkFramebuffer> framebuffers;
  VkSemaphore presentSemaphore;
  VkSemaphore renderSemaphore;
  VkFence renderFence;
  VkExtent2D windowExtent = {0, 0};

  void initVulkan();
  void initSwapchain();
  void initCommands();
  void initDefaultRenderpass();
  void initFramebuffers();
  void initSyncStructures();

  int frameNumber = 0;
};

}

#endif