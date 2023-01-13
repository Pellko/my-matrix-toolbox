#include "ShapeRenderer.hh"
#include "src/graphics/DescriptorAllocator.hh"
#include "src/graphics/DescriptorBuilder.hh"
#include "src/graphics/Polygon.hh"

namespace mymatrixtoolbox {

ShapeRenderer::ShapeRenderer(std::shared_ptr<Window> window) : window(window), descriptorAllocator(window), descriptorLayoutCache(window) {}
ShapeRenderer::~ShapeRenderer() {}

void ShapeRenderer::init() {
  initDescriptors();  
  initPipeline();
}

void ShapeRenderer::initPipeline() {
  VkShaderModule polygonVertexShader;
  if(!window->loadShaderModule("src/graphics/shaders/polygon.vert.spv", &polygonVertexShader)) {
    std::cout << "[Graphics Engine]: Error when loading the polygon vertex shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded polygon vertex shader" << std::endl;
  }

  VkShaderModule polygonFragmentShader;
  if(!window->loadShaderModule("src/graphics/shaders/polygon.frag.spv", &polygonFragmentShader)) {
    std::cout << "[Graphics Engine]: Error when loading the polygon fragment shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded polygon fragment shader" << std::endl;
  }

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = vkutil::pipelineLayoutCreateInfo();
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &cameraSetLayout;

  VK_CHECK(vkCreatePipelineLayout(window->getDevice(), &pipelineLayoutInfo, nullptr, &polygonPipelineLayout));

  vkutil::PipelineBuilder pipelineBuilder;
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, polygonVertexShader));
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, polygonFragmentShader));
  pipelineBuilder.vertexInputInfo = vkutil::vertexInputStateCreateInfo();
  pipelineBuilder.inputAssembly = vkutil::inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  vkutil::VertexInputDescription vertexDescription = PolygonVertex::getVertexDescription();
  pipelineBuilder.vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
  pipelineBuilder.vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
  pipelineBuilder.vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
  pipelineBuilder.vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();

  pipelineBuilder.viewport.x = 0.0f;
	pipelineBuilder.viewport.y = 0.0f;
	pipelineBuilder.viewport.width = (float) window->getExtent().width;
	pipelineBuilder.viewport.height = (float) window->getExtent().height;
	pipelineBuilder.viewport.minDepth = 0.0f;
	pipelineBuilder.viewport.maxDepth = 1.0f;

  pipelineBuilder.scissor.offset = { 0, 0 };
	pipelineBuilder.scissor.extent = window->getExtent();

  pipelineBuilder.rasterizer = vkutil::rasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
  pipelineBuilder.multisampling = vkutil::multisamplingStateCreateInfo();
  pipelineBuilder.colorBlendAttachment = vkutil::colorBlendAttachmentState();
  pipelineBuilder.pipelineLayout = polygonPipelineLayout;
  polygonPipeline = pipelineBuilder.build(window->getDevice(), window->getRenderPass());

  vkDestroyShaderModule(window->getDevice(), polygonVertexShader, nullptr);
  vkDestroyShaderModule(window->getDevice(), polygonFragmentShader, nullptr);

  window->getDeletionQueue().push_function([=]() {
    vkDestroyPipeline(window->getDevice(), polygonPipeline, nullptr);
    vkDestroyPipelineLayout(window->getDevice(), polygonPipelineLayout, nullptr);
  });

  test.vertices.resize(3);
	test.vertices[0].position = { 100, 100, 0.0f };
	test.vertices[1].position = {0, 100, 0.0f };
	test.vertices[2].position = { 0.f, 0, 0.0f };
	test.vertices[0].color = { 1.f, 0.f, 0.0f };
	test.vertices[1].color = { 0.f, 1.f, 0.0f };
	test.vertices[2].color = { 0.f, 0.f, 1.0f };
  test.upload(window);
}

void ShapeRenderer::initDescriptors() {
  cameraBuffer = window->createBuffer(sizeof(PolygonCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = cameraBuffer.buffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(PolygonCameraData);

  DescriptorBuilder::begin(window, &descriptorLayoutCache, &descriptorAllocator)
    .bindBuffer(0, &bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
    .build(cameraSet, cameraSetLayout);
}

void ShapeRenderer::draw() {
  glm::mat4 projection = glm::ortho<float>(0.0f, window->getExtent().width, 0.0f, window->getExtent().height);
  PolygonCameraData camera;
  camera.projection = projection;

  void* data;
  vmaMapMemory(window->getAllocator(), cameraBuffer.allocation, &data);
  memcpy(data, &camera, sizeof(PolygonCameraData));
  vmaUnmapMemory(window->getAllocator(), cameraBuffer.allocation);

  vkCmdBindPipeline(window->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, polygonPipeline);
  vkCmdBindDescriptorSets(window->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, polygonPipelineLayout, 0, 1, &cameraSet, 0, nullptr);
  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(window->getCommandBuffer(), 0, 1, &test.vertexBuffer.buffer, &offset);
  vkCmdDraw(window->getCommandBuffer(), test.vertices.size(), 1, 0, 0);
}

void ShapeRenderer::terminate() {
  descriptorLayoutCache.terminate();
  descriptorAllocator.terminate();
}

}