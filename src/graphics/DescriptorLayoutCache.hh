#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_LAYOUT_CACHE_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_DESCRIPTOR_LAYOUT_CACHE_H_

#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "Window.hh"

namespace mymatrixtoolbox {

class DescriptorLayoutCache {
 public:
  DescriptorLayoutCache(std::shared_ptr<Window> window);
  ~DescriptorLayoutCache();

  VkDescriptorSetLayout createDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info);
  void terminate();

  struct DescriptorLayoutInfo {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bool operator==(const DescriptorLayoutInfo&) const;
    size_t hash() const;
  };

 private:
  struct DescriptorLayoutHash {
    std::size_t operator()(const DescriptorLayoutInfo& k) const {
      return k.hash();
    }
  };
  std::shared_ptr<Window> window;
  std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCache;
};

}

#endif