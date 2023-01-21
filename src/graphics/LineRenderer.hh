#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_LINE_RENDERER_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_LINE_RENDERER_H_

#include <vulkan/vulkan.hpp>
#include "DescriptorBuilder.hh"
#include "Renderable.hh"
#include "Memory.hh"
#include "Vertex.hh"
#include "Window.hh"

namespace mymatrixtoolbox {

class LineRenderer : public Renderable {
 public:
  LineRenderer(std::shared_ptr<Window> window, int zIndex) : Renderable(zIndex), window(window) {}

  void init() override;
  void render() override;

  void addLineSegment(glm::vec2 a, glm::vec2 b, glm::vec3 color, float width);

 private:
  std::shared_ptr<Window> window;
  LineMesh lineMesh;
  LineInstanceBuffer lineInstanceBuffer;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
  vkmemory::AllocatedBuffer cameraBuffer;
  VkDescriptorSet cameraSet;
  VkDescriptorSetLayout cameraSetLayout;

  void initDescriptors();
  void initPipeline();
  void initMesh();
  void initInstances();
};

}

#endif