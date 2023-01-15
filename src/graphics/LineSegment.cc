#include "LineSegment.hh"
#include "src/graphics/VkUtil.hh"

namespace mymatrixtoolbox {

vkutil::VertexInputDescription LineSegmentVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(LineSegmentVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputBindingDescription instanceBinding = {};
  instanceBinding.binding = 1;
  instanceBinding.stride = sizeof(LineSegmentInstanceData);
  instanceBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
  description.bindings.push_back(instanceBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  positionAttribute.offset = offsetof(LineSegmentVertex, position);

  VkVertexInputAttributeDescription pointAAttribute = {};
  pointAAttribute.binding = 1;
  pointAAttribute.location = 1;
  pointAAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointAAttribute.offset = offsetof(LineSegmentInstanceData, pointA);

  VkVertexInputAttributeDescription pointBAttribute = {};
  pointBAttribute.binding = 1;
  pointBAttribute.location = 2;
  pointBAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointBAttribute.offset = offsetof(LineSegmentInstanceData, pointB);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(pointAAttribute);
  description.attributes.push_back(pointBAttribute);

  return description;
}

LineSegment::LineSegment() {}
LineSegment::~LineSegment() {}

void LineSegment::upload(std::shared_ptr<Window> window) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = vertices.size() * sizeof(LineSegmentVertex);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vmaallocInfo = {};
  vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &bufferInfo, &vmaallocInfo, &vertexBuffer.buffer, &vertexBuffer.allocation, nullptr));

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), vertexBuffer.buffer, vertexBuffer.allocation);
  });

  void* data;
  vmaMapMemory(window->getAllocator(), vertexBuffer.allocation, &data);
  memcpy(data, vertices.data(), vertices.size() * sizeof(LineSegmentVertex));
  vmaUnmapMemory(window->getAllocator(), vertexBuffer.allocation);
}

}