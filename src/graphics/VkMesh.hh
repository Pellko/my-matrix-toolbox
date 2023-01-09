#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VK_MESH_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VK_MESH_H_

#include "src/graphics/VkMemory.hh"
#include <vector>
#include <glm/vec3.hpp>

namespace mymatrixtoolbox {

struct VertexInputDescription {
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;
  VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;

  static VertexInputDescription getVertexDescription();
};

struct Mesh {
  std::vector<Vertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer;
};

}

#endif