#include "Polygon.hh"

namespace mymatrixtoolbox {

Polygon::Polygon() {}
Polygon::~Polygon() {}

void Polygon::upload(std::shared_ptr<Window> window) {
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

}
