#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_SHAPE_RENDERER_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_SHAPE_RENDERER_H_

#include <vulkan/vulkan.hpp>
#include <iostream>
#include "VkUtil.hh"
#include "Window.hh"
#include "Polygon.hh"

namespace mymatrixtoolbox {

class ShapeRenderer {
 public:
  ShapeRenderer(std::shared_ptr<Window> window);
  ~ShapeRenderer();

  void init();
  void draw();

 private:
  std::shared_ptr<Window> window;
  VkPipelineLayout polygonPipelineLayout;
  VkPipeline polygonPipeline;

  Polygon test;
};

}

#endif