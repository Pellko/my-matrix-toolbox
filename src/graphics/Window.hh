#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_

#include <string>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "vkb/VkBootstrap.h"
#include "vk_mem_alloc.h"
#include "VkUtil.hh"

namespace mymatrixtoolbox {

class Window {
 public:
  Window(std::string title);
  ~Window();

  void init();
  uint32_t begin();
  void end(uint32_t id);
  void terminate();
  bool shouldClose();

  VmaAllocator getAllocator() {
    return allocator;
  }

  vkutil::DeletionQueue& getDeletionQueue() {
    return deletionQueue;
  }

  VkDevice getDevice() {
    return device;
  }

  VkExtent2D getExtent() {
    return windowExtent;
  }

  VkRenderPass getRenderPass() {
    return renderPass;
  }

  VkCommandBuffer getCommandBuffer() {
    return mainCommandBuffer;
  }

  bool loadShaderModule(const char* filePath, VkShaderModule* outShaderModule);

 private:
  int width = 800;
  int height = 600;
  VkExtent2D windowExtent = {0, 0};
  std::string title;
  GLFWwindow* window;
  
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkQueue graphicsQueue;
  uint32_t graphicsQueueFamily;
  VkSwapchainKHR swapchain;
  VkFormat swapchainImageFormat;
  std::vector<VkImage> swapchainImages;
  std::vector<VkImageView> swapchainImageViews;
  VmaAllocator allocator;
  VkCommandPool commandPool;
  VkCommandBuffer mainCommandBuffer;
  VkRenderPass renderPass;
  std::vector<VkFramebuffer> framebuffers;
  VkSemaphore presentSemaphore;
  VkSemaphore renderSemaphore;
  VkFence renderFence;

  vkutil::DeletionQueue deletionQueue;

  void initVulkan();
  void initSwapchain();
  void initCommands();
  void initDefaultRenderpass();
  void initFramebuffers();
  void initSyncStructures();
};

}

#endif