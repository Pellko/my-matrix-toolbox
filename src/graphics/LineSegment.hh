#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_LINE_SEGMENT_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_LINE_SEGMENT_H_

#include <glm/glm.hpp>
#include "VkUtil.hh"
#include "Memory.hh"
#include "Window.hh"

namespace mymatrixtoolbox {

struct LineSegmentVertex {
  glm::vec2 position;

  static vkutil::VertexInputDescription getVertexDescription();
};

struct LineSegmentInstanceData {
  glm::vec2 pointA;
  glm::vec2 pointB;
};

class LineSegment {
 public:
  LineSegment();
  ~LineSegment();

  std::vector<LineSegmentVertex> vertices;
  vkmemory::AllocatedBuffer vertexBuffer;
  void upload(std::shared_ptr<Window> window);
};

}

#endif
