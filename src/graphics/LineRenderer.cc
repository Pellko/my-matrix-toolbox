#include "LineRenderer.hh"
#include "src/graphics/VkUtil.hh"

namespace mymatrixtoolbox {

void LineRenderer::init() {
  initPipeline();
  initMesh();
  initInstances();
}

void LineRenderer::render() {
  vkCmdBindPipeline(window->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  VkDeviceSize vertexOffset = 0;
  VkDeviceSize instanceOffset = 0;
  vkCmdBindVertexBuffers(window->getCommandBuffer(), 0, 1, &lineMesh.vertexBuffer.buffer, &vertexOffset);
  vkCmdBindVertexBuffers(window->getCommandBuffer(), 1, 1, &lineInstanceBuffer.instanceBuffer.buffer, &instanceOffset);
  vkCmdDraw(window->getCommandBuffer(), lineMesh.vertices.size(), lineInstanceBuffer.instances.size(), 0, 0);
}

void LineRenderer::addLineSegment(glm::vec2 a, glm::vec2 b) {
  lineInstanceBuffer.instances.push_back(LineInstance{
    .pointA = a,
    .pointB = b,
  });
}

void LineRenderer::initPipeline() {
  // (1) Load shaders
  VkShaderModule vertexShader;
  if(!window->loadShaderModule("src/graphics/shaders/line.vert.spv", &vertexShader)) {
    std::cout << "[Graphics Engine]: Error when loading the line vertex shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded the line vertex shader module" << std::endl;
  }

  VkShaderModule fragmentShader;
  if(!window->loadShaderModule("src/graphics/shaders/line.frag.spv", &fragmentShader)) {
    std::cout << "[Graphics Engine]: Error when loading the line fragment shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded the line fragment shader module" << std::endl;
  }

  // (2) Create pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  VK_CHECK(vkCreatePipelineLayout(window->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout));

  // (3) Create pipeline
  vkutil::PipelineBuilder pipelineBuilder;
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));  
  pipelineBuilder.vertexInputInfo = vkutil::vertexInputStateCreateInfo();
  pipelineBuilder.inputAssembly = vkutil::inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  vkutil::VertexInputDescription vertexDescription = LineVertex::getVertexDescription();
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
  pipelineBuilder.pipelineLayout = pipelineLayout;
  pipeline = pipelineBuilder.build(window->getDevice(), window->getRenderPass());

  vkDestroyShaderModule(window->getDevice(), vertexShader, nullptr);
  vkDestroyShaderModule(window->getDevice(), fragmentShader, nullptr);

  window->getDeletionQueue().push_function([=]() {
    vkDestroyPipeline(window->getDevice(), pipeline, nullptr);
    vkDestroyPipelineLayout(window->getDevice(), pipelineLayout, nullptr);
  });
}

void LineRenderer::initMesh() {
  lineMesh.vertices.resize(6);
  lineMesh.vertices[0].position = {0, -0.5f};
  lineMesh.vertices[1].position = {1, -0.5f};
  lineMesh.vertices[2].position = {1, 0.5f};
  lineMesh.vertices[3].position = {0, -0.5f};
  lineMesh.vertices[4].position = {1, 0.5f};
  lineMesh.vertices[5].position = {0, 0.5f};
  lineMesh.upload(window);
}

void LineRenderer::initInstances() {
  lineInstanceBuffer.upload(window);
}

}