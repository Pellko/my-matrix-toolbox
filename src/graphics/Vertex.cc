#include "Vertex.hh"

namespace mymatrixtoolbox {

vkutil::VertexInputDescription PolygonVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(PolygonVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
  positionAttribute.offset = offsetof(PolygonVertex, position);

  VkVertexInputAttributeDescription colorAttribute = {};
  colorAttribute.binding = 0;
  colorAttribute.location = 1;
  colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
  colorAttribute.offset = offsetof(PolygonVertex, color);

  VkVertexInputAttributeDescription uvAttribute = {};
  uvAttribute.binding = 0;
  uvAttribute.location = 2;
  uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  uvAttribute.offset = offsetof(PolygonVertex, uv);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(colorAttribute);
  description.attributes.push_back(uvAttribute);

  return description;
}

void PolygonMesh::upload(std::shared_ptr<Window> window) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = vertices.size() * sizeof(PolygonVertex);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  
  VmaAllocationCreateInfo vmaallocInfo = {};
  vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &bufferInfo, &vmaallocInfo, &vertexBuffer.buffer, &vertexBuffer.allocation, nullptr));

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), vertexBuffer.buffer, vertexBuffer.allocation);
  });

  void* data;
  vmaMapMemory(window->getAllocator(), vertexBuffer.allocation, &data);
  memcpy(data, vertices.data(), vertices.size() * sizeof(PolygonVertex));
  vmaUnmapMemory(window->getAllocator(), vertexBuffer.allocation);
}

vkutil::VertexInputDescription LineVertex::getVertexDescription() {
  vkutil::VertexInputDescription description;

  VkVertexInputBindingDescription mainBinding = {};
  mainBinding.binding = 0;
  mainBinding.stride = sizeof(LineVertex);
  mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  description.bindings.push_back(mainBinding);

  VkVertexInputBindingDescription instanceBinding = {};
  instanceBinding.binding = 1;
  instanceBinding.stride = sizeof(LineInstance);
  instanceBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
  description.bindings.push_back(instanceBinding);

  VkVertexInputAttributeDescription positionAttribute = {};
  positionAttribute.binding = 0;
  positionAttribute.location = 0;
  positionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  positionAttribute.offset = offsetof(LineVertex, position);

  VkVertexInputAttributeDescription pointAAttribute = {};
  pointAAttribute.binding = 1;
  pointAAttribute.location = 1;
  pointAAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointAAttribute.offset = offsetof(LineInstance, pointA);

  VkVertexInputAttributeDescription pointBAttribute = {};
  pointBAttribute.binding = 1;
  pointBAttribute.location = 2;
  pointBAttribute.format = VK_FORMAT_R32G32_SFLOAT;
  pointBAttribute.offset = offsetof(LineInstance, pointB);

  description.attributes.push_back(positionAttribute);
  description.attributes.push_back(pointAAttribute);
  description.attributes.push_back(pointBAttribute);

  return description;
}

void LineMesh::upload(std::shared_ptr<Window> window) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = vertices.size() * sizeof(LineVertex);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vmaallocInfo = {};
  vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &bufferInfo, &vmaallocInfo, &vertexBuffer.buffer, &vertexBuffer.allocation, nullptr));

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), vertexBuffer.buffer, vertexBuffer.allocation);
  });

  void* data;
  vmaMapMemory(window->getAllocator(), vertexBuffer.allocation, &data);
  memcpy(data, vertices.data(), vertices.size() * sizeof(LineVertex));
  vmaUnmapMemory(window->getAllocator(), vertexBuffer.allocation);
}

void LineInstanceBuffer::upload(std::shared_ptr<Window> window) {
  VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = instances.size() * sizeof(LineInstance);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vmaallocInfo = {};
  vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &bufferInfo, &vmaallocInfo, &instanceBuffer.buffer, &instanceBuffer.allocation, nullptr));

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), instanceBuffer.buffer, instanceBuffer.allocation);
  });

  void* data;
  vmaMapMemory(window->getAllocator(), instanceBuffer.allocation, &data);
  memcpy(data, instances.data(), instances.size() * sizeof(LineInstance));
  vmaUnmapMemory(window->getAllocator(), instanceBuffer.allocation);
}

}