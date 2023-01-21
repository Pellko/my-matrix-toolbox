#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VERTEX_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VERTEX_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Memory.hh"
#include "VkUtil.hh"
#include "Window.hh"

namespace mymatrixtoolbox {

namespace vertex {

template<typename T>
void uploadMesh(std::shared_ptr<Window> window, std::vector<T>& vertices, vkmemory::AllocatedBuffer* target) {
  const size_t bufferSize = vertices.size() * sizeof(T);

	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;

	stagingBufferInfo.size = bufferSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

  vkmemory::AllocatedBuffer stagingBuffer;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer.buffer, &stagingBuffer.allocation, nullptr));

  void* data;
  vmaMapMemory(window->getAllocator(), stagingBuffer.allocation, &data);
  memcpy(data, vertices.data(), vertices.size() * sizeof(T));
  vmaUnmapMemory(window->getAllocator(), stagingBuffer.allocation);

	VkBufferCreateInfo vertexBufferInfo = {};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.pNext = nullptr;
  vertexBufferInfo.size = bufferSize;
	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  VK_CHECK(vmaCreateBuffer(window->getAllocator(), &vertexBufferInfo, &vmaallocInfo, &target->buffer, &target->allocation, nullptr));

  window->immediateSubmit([=](VkCommandBuffer cmd) {
    VkBufferCopy copy = {};
    copy.srcOffset = 0;
    copy.dstOffset = 0;
    copy.size = bufferSize;
    vkCmdCopyBuffer(cmd, stagingBuffer.buffer, target->buffer, 1, &copy);
  });

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyBuffer(window->getAllocator(), target->buffer, target->allocation);
  });
  vmaDestroyBuffer(window->getAllocator(), stagingBuffer.buffer, stagingBuffer.allocation);
}

}

struct CameraData {
  glm::mat4 projection;

  static CameraData getStandardOrtho(std::shared_ptr<Window> window);
};

struct PolygonVertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;

  static vkutil::VertexInputDescription getVertexDescription();
};

class PolygonMesh {
 public:
  PolygonMesh() {}
  ~PolygonMesh() {}

  std::vector<PolygonVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer = {};
};

struct LineVertex {
  glm::vec2 position;

  static vkutil::VertexInputDescription getVertexDescription();
};

struct LineInstance {
  glm::vec2 pointA;
  glm::vec2 pointB;
  glm::vec3 color;
  float width;
};

class LineMesh {
 public:
  LineMesh() {}
  ~LineMesh() {}

  std::vector<LineVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer = {};
};

class LineInstanceBuffer {
 public:
  LineInstanceBuffer() {}
  ~LineInstanceBuffer() {}

  std::vector<LineInstance> instances;
  vkmemory::AllocatedBuffer instanceBuffer = {};
};

struct FontVertex {
  glm::vec2 position;
  glm::vec2 uv;

  static vkutil::VertexInputDescription getVertexDescription();
};

class FontMesh {
 public:
  FontMesh() {}
  ~FontMesh() {}

  std::vector<FontVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer = {};
};

}

#endif