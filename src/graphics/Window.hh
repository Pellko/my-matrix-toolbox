#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_WINDOW_H_

#include <string>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "vkb/VkBootstrap.h"
#include "vk_mem_alloc.h"
#include "DescriptorAllocator.hh"
#include "DescriptorLayoutCache.hh"
#include "VkUtil.hh"
#include "Memory.hh"
#include "Renderable.hh"

namespace mymatrixtoolbox {

class Window {
 public:
  Window(std::string title);
  ~Window();

  void init();
  uint32_t begin();
  void drawRenderables();
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

  GLFWwindow* getGlfwHandle() {
    return window;
  }

  DescriptorAllocator& getDescriptorAllocator() {
    return descriptorAllocator;
  }

  DescriptorLayoutCache& getDescriptorLayoutCache() {
    return descriptorLayoutCache;
  }

  void addRenderable(std::shared_ptr<Renderable> renderable) {
    renderables.push_back(renderable);
    std::sort(renderables.begin(), renderables.end(), [](std::shared_ptr<Renderable> a, std::shared_ptr<Renderable> b) {
      return a->getZIndex() < b->getZIndex();
    });
  }

  static void initGLFW();
  static void terminateGLFW();

  void immediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);
  bool loadShaderModule(const char* filePath, VkShaderModule* outShaderModule);
  vkmemory::AllocatedBuffer createBuffer(size_t allocationSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

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

  vkutil::UploadContext uploadContext;
  vkutil::DeletionQueue deletionQueue;
  DescriptorAllocator descriptorAllocator;
  DescriptorLayoutCache descriptorLayoutCache;
  std::vector<std::shared_ptr<Renderable>> renderables;

  void initVulkan();
  void initSwapchain();
  void initCommands();
  void initDefaultRenderpass();
  void initFramebuffers();
  void initSyncStructures();
};

}

#endif