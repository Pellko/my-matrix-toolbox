#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VK_POLYGON_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VK_POLYGON_H_

#include <glm/glm.hpp>
#include "Memory.hh"
#include "VkUtil.hh"
#include "Window.hh"

namespace mymatrixtoolbox {

struct PolygonVertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;

  static vkutil::VertexInputDescription getVertexDescription() {
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
};

struct PolygonCameraData {
  glm::mat4 projection;
};

class Polygon {
 public:
  Polygon();
  ~Polygon();

  std::vector<PolygonVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer;
  void upload(std::shared_ptr<Window> window);
};

}

#endif