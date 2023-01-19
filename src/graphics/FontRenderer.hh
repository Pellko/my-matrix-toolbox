#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_FONT_RENDERER_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_FONT_RENDERER_H_

#include <vulkan/vulkan.hpp>
#include "DescriptorBuilder.hh"
#include "Renderable.hh"
#include "Window.hh"
#include "Vertex.hh"
#include "Font.hh"

namespace mymatrixtoolbox {

class FontRenderer : public Renderable {
 public:
  FontRenderer(std::shared_ptr<Window> window, int zIndex);
  ~FontRenderer();

  void init() override;
  void render() override;

 private:
  std::shared_ptr<Window> window;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
  VkSampler textureSampler;
  vkmemory::AllocatedBuffer cameraBuffer;
  VkDescriptorSet cameraSet;
  VkDescriptorSetLayout cameraSetLayout;
  FontMesh fontMesh;
  Font font;

  void initSampler();
  void initDescriptors();
  void initPipeline();
  void initMesh();

  void addCharacter(char c, int x, int y, int width, int height);
  void drawString(std::string str, int x, int y);

};

}

#endif
