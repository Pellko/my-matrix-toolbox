#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VERTEX_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VERTEX_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Memory.hh"
#include "VkUtil.hh"
#include "Window.hh"

namespace mymatrixtoolbox {

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
  vkmemory::AllocatedBuffer vertexBuffer;
  void upload(std::shared_ptr<Window> window);
};

struct LineVertex {
  glm::vec2 position;

  static vkutil::VertexInputDescription getVertexDescription();
};

struct LineInstance {
  glm::vec2 pointA;
  glm::vec2 pointB;
};

class LineMesh {
 public:
  LineMesh() {}
  ~LineMesh() {}

  std::vector<LineVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer;
  void upload(std::shared_ptr<Window> window);
};

class LineInstanceBuffer {
 public:
  LineInstanceBuffer() {}
  ~LineInstanceBuffer() {}

  std::vector<LineInstance> instances;
  vkmemory::AllocatedBuffer instanceBuffer;
  void upload(std::shared_ptr<Window> window);
};

}

#endif