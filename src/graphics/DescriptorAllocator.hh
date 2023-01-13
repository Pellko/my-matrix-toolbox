#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_ALLOCATOR_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_ALLOCATOR_H_

#include <vector>
#include <vulkan/vulkan.hpp>
#include "Window.hh"

namespace mymatrixtoolbox {

class DescriptorAllocator {
 public:
  DescriptorAllocator(std::shared_ptr<Window> window);
  ~DescriptorAllocator();

  struct PoolSizes {
    std::vector<std::pair<VkDescriptorType, float>> sizes = {
      { VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
    };
  };

  void resetPools();
  bool allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);
  void terminate();

 private:
  std::shared_ptr<Window> window;
  VkDescriptorPool grabPool();
  VkDescriptorPool currentPool{VK_NULL_HANDLE};
  PoolSizes descriptorSizes;
  std::vector<VkDescriptorPool> usedPools;
  std::vector<VkDescriptorPool> freePools;
};

}

#endif