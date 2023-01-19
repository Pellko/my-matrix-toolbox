#include "FontRenderer.hh"

namespace mymatrixtoolbox {

FontRenderer::FontRenderer(std::shared_ptr<Window> window, int zIndex) : Renderable(zIndex), window(window) {}
FontRenderer::~FontRenderer() {}

void FontRenderer::init() {
  font.init(window);
  initSampler();
  initDescriptors();
  initPipeline();
  initMesh();
}

void FontRenderer::render() {
  CameraData cameraData = CameraData::getStandardOrtho(window);
  void* data;
  vmaMapMemory(window->getAllocator(), cameraBuffer.allocation, &data);
  memcpy(data, &cameraData, sizeof(CameraData));
  vmaUnmapMemory(window->getAllocator(), cameraBuffer.allocation);

  vkCmdBindPipeline(window->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vkCmdBindDescriptorSets(window->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &cameraSet, 0, nullptr);
  VkDeviceSize vertexOffset = 0;
  vkCmdBindVertexBuffers(window->getCommandBuffer(), 0, 1, &fontMesh.vertexBuffer.buffer, &vertexOffset);
  vkCmdDraw(window->getCommandBuffer(), fontMesh.vertices.size(), 1, 0, 0);
}

void FontRenderer::initSampler() {
  VkSamplerCreateInfo samplerInfo = {};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;
  VK_CHECK(vkCreateSampler(window->getDevice(), &samplerInfo, nullptr, &textureSampler));

  window->getDeletionQueue().push_function([=]() {
    vkDestroySampler(window->getDevice(), textureSampler, nullptr);
  });
}

void FontRenderer::initDescriptors() {
  cameraBuffer = window->createBuffer(sizeof(CameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = cameraBuffer.buffer;
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(CameraData);

  VkDescriptorImageInfo imageInfo = {};
  imageInfo.imageView = font.getImageView();
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.sampler = textureSampler;

  DescriptorBuilder::begin(window, &window->getDescriptorLayoutCache(), &window->getDescriptorAllocator())
    .bindBuffer(0, &bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
    .bindImage(1, &imageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    .build(cameraSet, cameraSetLayout);

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), cameraBuffer.buffer, cameraBuffer.allocation);
  });
}

void FontRenderer::initPipeline() {
  // (1) Load shaders
  VkShaderModule vertexShader;
  if(!window->loadShaderModule("src/graphics/shaders/text.vert.spv", &vertexShader)) {
    std::cout << "[Graphics Engine]: Error when loading the text vertex shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded the text vertex shader module" << std::endl;
  }

  VkShaderModule fragmentShader;
  if(!window->loadShaderModule("src/graphics/shaders/text.frag.spv", &fragmentShader)) {
    std::cout << "[Graphics Engine]: Error when loading the text fragment shader module" << std::endl;
  } else {
    std::cout << "[Graphics Engine]: Sucessfully loaded the text fragment shader module" << std::endl;
  }
  
  // (2) Create pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &cameraSetLayout;

  VK_CHECK(vkCreatePipelineLayout(window->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout));

  // (3) Create pipeline
  vkutil::PipelineBuilder pipelineBuilder;
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));
  pipelineBuilder.shaderStages.push_back(vkutil::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));  
  pipelineBuilder.vertexInputInfo = vkutil::vertexInputStateCreateInfo();
  pipelineBuilder.inputAssembly = vkutil::inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  vkutil::VertexInputDescription vertexDescription = FontVertex::getVertexDescription();
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

void FontRenderer::initMesh() {
  int x = 0;
  int y = 0;
  int width = 20;
  int height = 20;

  Character& character = font.getCharacter('a');
  float xs = character.offset.x;
  float xe = character.offset.x + character.size.x;
  float ys = character.offset.y;
  float ye = character.offset.y + character.size.y;

  fontMesh.vertices.resize(6);
  fontMesh.vertices[0].position = {x, y};
  fontMesh.vertices[0].uv = {xs, ys};

  fontMesh.vertices[1].position = {x+width, y};
  fontMesh.vertices[1].uv = {xe, ys};

  fontMesh.vertices[2].position = {x+width, y+height};
  fontMesh.vertices[2].uv = {xe, ye};

  fontMesh.vertices[3].position = {x+width, y+height};
  fontMesh.vertices[3].uv = {xe, ye};

  fontMesh.vertices[4].position = {x, y+height};
  fontMesh.vertices[4].uv = {xs, ye};

  fontMesh.vertices[5].position = {x, y};
  fontMesh.vertices[5].uv = {xs, ys};
  
  vertex::uploadMesh<FontVertex>(window, fontMesh.vertices, &fontMesh.vertexBuffer);
}

} 