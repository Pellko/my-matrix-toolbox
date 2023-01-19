#include "DescriptorBuilder.hh"

namespace mymatrixtoolbox {

DescriptorBuilder DescriptorBuilder::begin(std::shared_ptr<Window> window, DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator) {
  DescriptorBuilder builder;
  builder.window = window;
  builder.cache = layoutCache;
  builder.alloc = allocator;
  return builder;
}

DescriptorBuilder& DescriptorBuilder::bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags) {
  VkDescriptorSetLayoutBinding bindingInfo = {};
  bindingInfo.descriptorCount = 1;
  bindingInfo.descriptorType = type;
  bindingInfo.pImmutableSamplers = nullptr;
  bindingInfo.stageFlags = stageFlags;
  bindingInfo.binding = binding;
  bindings.push_back(bindingInfo);

  VkWriteDescriptorSet write = {};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.pNext = nullptr;
  write.descriptorCount = 1;
  write.descriptorType = type;
  write.pBufferInfo = bufferInfo;
  write.dstBinding = binding;
  writes.push_back(write);

  return *this;
}

DescriptorBuilder& DescriptorBuilder::bindImage(uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags) {
  VkDescriptorSetLayoutBinding bindingInfo = {};
  bindingInfo.descriptorCount = 1;
  bindingInfo.descriptorType = type;
  bindingInfo.pImmutableSamplers = nullptr;
  bindingInfo.stageFlags = stageFlags;
  bindingInfo.binding = binding;
  bindings.push_back(bindingInfo);

  VkWriteDescriptorSet write = {};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.pNext = nullptr;

  write.descriptorCount = 1;
  write.descriptorType = type;
  write.pImageInfo = imageInfo;
  write.dstBinding = binding;
  writes.push_back(write);

  return *this;
}

bool DescriptorBuilder::build(VkDescriptorSet& set, VkDescriptorSetLayout& layout) {
  VkDescriptorSetLayoutCreateInfo layoutInfo = {};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.pNext = nullptr;

  layoutInfo.pBindings = bindings.data();
  layoutInfo.bindingCount = bindings.size();

  layout = cache->createDescriptorLayout(&layoutInfo);

  bool success = alloc->allocate(&set, layout);
  if(!success) return false;

  for(VkWriteDescriptorSet& w : writes) {
    w.dstSet = set;
  }
  vkUpdateDescriptorSets(window->getDevice(), writes.size(), writes.data(), 0, nullptr);
  return true;
}

}