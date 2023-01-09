#include "Window.hh"
#include "VkUtil.hh"
#include <math.h>
#include <fstream>

namespace mymatrixtoolbox {

static void errorCallback(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

Window::Window(std::string title) : title(title) {}
Window::~Window() {}

void Window::init() {
  glfwSetErrorCallback(errorCallback);

  if(!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  int framebufferWidth;
  int framebufferHeight;
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
  windowExtent.width = framebufferWidth;
  windowExtent.height = framebufferHeight;

  initVulkan();
  initSwapchain();
  initCommands();
  initDefaultRenderpass();
  initFramebuffers();
  initSyncStructures();
  initPipelines();
  initMeshes();
}

void Window::run() {
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    draw();
  }
}

void Window::draw() {
  // Wait for previous frame
  VK_CHECK(vkWaitForFences(device, 1, &renderFence, true, 1000000000));
  VK_CHECK(vkResetFences(device, 1, &renderFence));

  // Aquire next image to render to
  uint32_t swapchainImageIndex;
  VK_CHECK(vkAcquireNextImageKHR(device, swapchain, 1000000000, presentSemaphore, nullptr, &swapchainImageIndex));

  // Initialize now command buffer
  VK_CHECK(vkResetCommandBuffer(mainCommandBuffer, 0));
  VkCommandBufferBeginInfo cmdBeginInfo = {};
  cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmdBeginInfo.pNext = nullptr;
  cmdBeginInfo.pInheritanceInfo = nullptr;
  cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  VK_CHECK(vkBeginCommandBuffer(mainCommandBuffer, &cmdBeginInfo));

  // Create a renderpass
  VkClearValue clearValue;
	float flash = abs(sin(frameNumber / 120.f));
	clearValue.color = { { 0.0f, 0.0f, flash, 1.0f } };

  VkRenderPassBeginInfo rpInfo = {};
  rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpInfo.pNext = nullptr;
  rpInfo.renderPass = renderPass;
  rpInfo.renderArea.offset.x = 0;
  rpInfo.renderArea.offset.y = 0;
  rpInfo.renderArea.extent = windowExtent;
  rpInfo.framebuffer = framebuffers[swapchainImageIndex];

  rpInfo.clearValueCount = 1;
  rpInfo.pClearValues = &clearValue;

  // Draw
  vkCmdBeginRenderPass(mainCommandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(mainCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);

  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(mainCommandBuffer, 0, 1, &triangleMesh.vertexBuffer.buffer, &offset);
  vkCmdDraw(mainCommandBuffer, triangleMesh.vertices.size(), 1, 0, 0);

  vkCmdEndRenderPass(mainCommandBuffer);
  VK_CHECK(vkEndCommandBuffer(mainCommandBuffer));

  // Submit to graphics queue
  VkSubmitInfo submit = {};
  submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit.pNext = nullptr;
  VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  submit.pWaitDstStageMask = &waitStage;

  submit.waitSemaphoreCount = 1;
  submit.pWaitSemaphores = &presentSemaphore;

  submit.signalSemaphoreCount = 1;
  submit.pSignalSemaphores = &renderSemaphore;
  
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &mainCommandBuffer;

  VK_CHECK(vkQueueSubmit(graphicsQueue, 1, &submit, renderFence));

  // Present to display
  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.pSwapchains = &swapchain;
  presentInfo.swapchainCount = 1;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &renderSemaphore;

  presentInfo.pImageIndices = &swapchainImageIndex;
  
  VK_CHECK(vkQueuePresentKHR(graphicsQueue, &presentInfo));

  frameNumber++;
}

void Window::initVulkan() {
  vkb::InstanceBuilder builder(vkGetInstanceProcAddr);

  uint32_t requiredExtensionsCount = 0;
  const char** requiredExtensions;
  requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

  builder = builder.set_app_name(title.c_str())
    .set_engine_name("My Matrix Toolbox")
    .set_engine_version(0, 1)
    .set_app_version(0, 1)
    .request_validation_layers(true)
    .require_api_version(1, 1, 0)
    .use_default_debug_messenger();

  for(int i=0;i<requiredExtensionsCount;i++) {
    builder = builder.enable_extension(requiredExtensions[i]);
  }
  
  auto instRet = builder.build();

  if(!instRet) {
    std::cerr << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
    exit(EXIT_FAILURE);
  }

  vkb::Instance vkbInstance = instRet.value();
  instance = vkbInstance.instance;
  debugMessenger = vkbInstance.debug_messenger;

  VkResult err = glfwCreateWindowSurface(instance, window, nullptr, &surface);
  if(err) {
    std::cerr << "Failed to create window surface" << std::endl;
    exit(EXIT_FAILURE);
  }
  vkb::PhysicalDeviceSelector selector{ vkbInstance };
  vkb::PhysicalDevice vkbPhysicalDevice = selector
    .set_minimum_version(1, 1)
    .set_surface(surface)
    .select()
    .value();

  vkb::DeviceBuilder deviceBuilder{ vkbPhysicalDevice };
  vkb::Device vkbDevice = deviceBuilder.build().value();
  device = vkbDevice.device;
  physicalDevice = vkbPhysicalDevice.physical_device;

  graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
  graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

  VmaAllocatorCreateInfo allocatorInfo = {};
  allocatorInfo.physicalDevice = physicalDevice;
  allocatorInfo.device = device;
  allocatorInfo.instance = instance;
  vmaCreateAllocator(&allocatorInfo, &allocator);
}

void Window::initSwapchain() {
  vkb::SwapchainBuilder swapchainBuilder{physicalDevice, device, surface};
  vkb::Swapchain vkbSwapchain = swapchainBuilder
    .use_default_format_selection()
    .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
    .set_desired_extent(windowExtent.width, windowExtent.height)
    .build()
    .value();

  swapchain = vkbSwapchain.swapchain;
  swapchainImages = vkbSwapchain.get_images().value();
  swapchainImageViews = vkbSwapchain.get_image_views().value();
  swapchainImageFormat = vkbSwapchain.image_format;
  deletionQueue.push_function([=]() {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  });
}

void Window::initCommands() {
  VkCommandPoolCreateInfo commandPoolInfo = vkutil::commandPoolCreateInfo(graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  VK_CHECK(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool));

  VkCommandBufferAllocateInfo cmdAllocInfo = vkutil::commandBufferAllocateInfo(commandPool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  VK_CHECK(vkAllocateCommandBuffers(device, &cmdAllocInfo, &mainCommandBuffer));

  deletionQueue.push_function([=]() {
    vkDestroyCommandPool(device, commandPool, nullptr);
  });
}

void Window::initDefaultRenderpass() {
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format = swapchainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));

  deletionQueue.push_function([=]() {
    vkDestroyRenderPass(device, renderPass, nullptr);
  });
}

void Window::initFramebuffers() {
  VkFramebufferCreateInfo frameBufferInfo = {};
  frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  frameBufferInfo.pNext = nullptr;
  frameBufferInfo.renderPass = renderPass;
  frameBufferInfo.attachmentCount = 1;
  frameBufferInfo.width = windowExtent.width;
  frameBufferInfo.height = windowExtent.height;
  frameBufferInfo.layers = 1;

  const uint32_t swapchainImageCount = swapchainImages.size();
  framebuffers = std::vector<VkFramebuffer>(swapchainImageCount);
  for(int i=0;i<swapchainImageCount;i++) {
    frameBufferInfo.pAttachments = &swapchainImageViews[i];
    VK_CHECK(vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &framebuffers[i]));

    deletionQueue.push_function([=]() {
      vkDestroyFramebuffer(device, framebuffers[i], nullptr);
      vkDestroyImageView(device, swapchainImageViews[i], nullptr);
    });
  }
}

void Window::initSyncStructures() {
  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.pNext = nullptr;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &renderFence));

  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphoreCreateInfo.pNext = nullptr;
  semaphoreCreateInfo.flags = 0;

  VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &presentSemaphore));
  VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderSemaphore));

  deletionQueue.push_function([=]() {
    vkDestroySemaphore(device, presentSemaphore, nullptr);
    vkDestroySemaphore(device, renderSemaphore, nullptr);
  });
}

void Window::initPipelines() {
  VkShaderModule triangleVertexShader;
  if(!loadShaderModule("src/graphics/shaders/triangle.vert.spv", &triangleVertexShader)) {
    std::cout << "Error when loading the triangle vertex shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded triangle vertex shader" << std::endl;
  }

  VkShaderModule triangleFragmentShader;
  if(!loadShaderModule("src/graphics/shaders/triangle.frag.spv", &triangleFragmentShader)) {
    std::cout << "Error when loading the triangle fragment shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded triangle fragment shader" << std::endl;
  }

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = vkutil::pipelineLayoutCreateInfo();
  VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &trianglePipelineLayout));

  vkutil::PipelineBuilder pipelineBuilder;
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertexShader));
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragmentShader));
  pipelineBuilder.vertexInputInfo = vkutil::vertexInputStateCreateInfo();
  pipelineBuilder.inputAssembly = vkutil::inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  VertexInputDescription vertexDescription = Vertex::getVertexDescription();
  pipelineBuilder.vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
  pipelineBuilder.vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
  pipelineBuilder.vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
  pipelineBuilder.vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();

  pipelineBuilder.viewport.x = 0.0f;
	pipelineBuilder.viewport.y = 0.0f;
	pipelineBuilder.viewport.width = (float) windowExtent.width;
	pipelineBuilder.viewport.height = (float) windowExtent.height;
	pipelineBuilder.viewport.minDepth = 0.0f;
	pipelineBuilder.viewport.maxDepth = 1.0f;

  pipelineBuilder.scissor.offset = { 0, 0 };
	pipelineBuilder.scissor.extent = windowExtent;

  pipelineBuilder.rasterizer = vkutil::rasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
  pipelineBuilder.multisampling = vkutil::multisamplingStateCreateInfo();
  pipelineBuilder.colorBlendAttachment = vkutil::colorBlendAttachmentState();
  pipelineBuilder.pipelineLayout = trianglePipelineLayout;
  trianglePipeline = pipelineBuilder.build(device, renderPass);

  vkDestroyShaderModule(device, triangleVertexShader, nullptr);
  vkDestroyShaderModule(device, triangleFragmentShader, nullptr);

  deletionQueue.push_function([=]() {
    vkDestroyPipeline(device, trianglePipeline, nullptr);
    vkDestroyPipelineLayout(device, trianglePipelineLayout, nullptr);
  });
}

void Window::initMeshes() {
  triangleMesh.vertices.resize(3);
	triangleMesh.vertices[0].position = { 1.f, 1.f, 0.0f };
	triangleMesh.vertices[1].position = {-1.f, 1.f, 0.0f };
	triangleMesh.vertices[2].position = { 0.f,-1.f, 0.0f };
	triangleMesh.vertices[0].color = { 0.f, 1.f, 0.0f };
	triangleMesh.vertices[1].color = { 0.f, 1.f, 0.0f };
	triangleMesh.vertices[2].color = { 0.f, 1.f, 0.0f };

  uploadMesh(triangleMesh);
}

void Window::uploadMesh(Mesh& mesh) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = mesh.vertices.size() * sizeof(Vertex);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vmaallocInfo = {};
  vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  VK_CHECK(vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo, &mesh.vertexBuffer.buffer, &mesh.vertexBuffer.allocation, nullptr));

  deletionQueue.push_function([=]() {
    vmaDestroyBuffer(allocator, mesh.vertexBuffer.buffer, mesh.vertexBuffer.allocation);
  });

  void* data;
  vmaMapMemory(allocator, mesh.vertexBuffer.allocation, &data);
  memcpy(data, mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
  vmaUnmapMemory(allocator, mesh.vertexBuffer.allocation);
}

bool Window::loadShaderModule(const char* filePath, VkShaderModule* outShaderModule) {
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  if(!file.is_open()) {
    return false;
  }

  size_t fileSize = (size_t) file.tellg();
  std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
  file.seekg(0);
  file.read((char*) buffer.data(), fileSize);
  file.close();

  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;

  createInfo.codeSize = buffer.size() * sizeof(uint32_t);
  createInfo.pCode = buffer.data();

  VkShaderModule shaderModule;
  if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    return false;
  }
  *outShaderModule = shaderModule;
  return true;
}

void Window::terminate() {
  vkWaitForFences(device, 1, &renderFence, true, 1000000000);
  deletionQueue.flush();
  vmaDestroyAllocator(allocator);
  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkb::destroy_debug_utils_messenger(instance, debugMessenger);
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

}