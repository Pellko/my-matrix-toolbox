#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_SHAPE_RENDERER_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_SHAPE_RENDERER_H_

#include <vulkan/vulkan.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "VkUtil.hh"
#include "Window.hh"
#include "Vertex.hh"
#include "Memory.hh"
#include "DescriptorAllocator.hh"
#include "DescriptorLayoutCache.hh"
#include "DescriptorBuilder.hh"

namespace mymatrixtoolbox {

class ShapeRenderer {
 public:
  ShapeRenderer(std::shared_ptr<Window> window);
  ~ShapeRenderer();

  void init();
  void draw();
  void terminate();

 private:
  std::shared_ptr<Window> window;
  VkPipelineLayout polygonPipelineLayout;
  VkPipeline polygonPipeline;
  // VkPipelineLayout linePipelineLayout;
  // VkPipeline linePipeline;
  // DescriptorAllocator descriptorAllocator;
  // DescriptorLayoutCache descriptorLayoutCache;

  void initPolygonPipeline();
  void initDescriptors();

  VkDescriptorSet cameraSet;
  VkDescriptorSetLayout cameraSetLayout;
  vkmemory::AllocatedBuffer cameraBuffer;
  PolygonMesh test;

  // LineSegment lineSegment;
  // vkmemory::AllocatedBuffer lineInstanceBuffer;
  // std::vector<LineSegmentInstanceData> lineInstanceData;
};

}

#endif