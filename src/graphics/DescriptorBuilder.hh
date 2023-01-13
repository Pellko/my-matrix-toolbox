#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_BUILDER_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_BUILDER_H_

#include "DescriptorLayoutCache.hh"
#include "DescriptorAllocator.hh"
#include "src/graphics/Window.hh"

namespace mymatrixtoolbox {

class DescriptorBuilder {
 public:
  static DescriptorBuilder begin(std::shared_ptr<Window> window, DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator);

  DescriptorBuilder& bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

  bool build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);

 private:
  std::shared_ptr<Window> window;
  std::vector<VkWriteDescriptorSet> writes;
  std::vector<VkDescriptorSetLayoutBinding> bindings;

  DescriptorLayoutCache* cache;
  DescriptorAllocator* alloc;
};

}

#endif