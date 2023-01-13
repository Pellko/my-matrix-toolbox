#include "DescriptorAllocator.hh"

namespace mymatrixtoolbox {

VkDescriptorPool createPool(VkDevice device, const DescriptorAllocator::PoolSizes& poolSizes, int count, VkDescriptorPoolCreateFlags flags) {
  std::vector<VkDescriptorPoolSize> sizes;
  sizes.reserve(poolSizes.sizes.size());
  for (auto sz : poolSizes.sizes) {
    sizes.push_back({ sz.first, uint32_t(sz.second * count) });
  }
  VkDescriptorPoolCreateInfo poolInfo = {};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.flags = flags;
  poolInfo.maxSets = count;
  poolInfo.poolSizeCount = (uint32_t)sizes.size();
  poolInfo.pPoolSizes = sizes.data();

  VkDescriptorPool descriptorPool;
  vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);

  return descriptorPool;
}

DescriptorAllocator::DescriptorAllocator(std::shared_ptr<Window> window) : window(window) {}
DescriptorAllocator::~DescriptorAllocator() {}

VkDescriptorPool DescriptorAllocator::grabPool() {
  if(freePools.size() > 0) {
    VkDescriptorPool pool = freePools.back();
    freePools.pop_back();
    return pool;
  } else {
    return createPool(window->getDevice(), descriptorSizes, 1000, 0);
  }
}

bool DescriptorAllocator::allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout) {
  if(currentPool == VK_NULL_HANDLE) {
    currentPool = grabPool();
    usedPools.push_back(currentPool);
  }

  VkDescriptorSetAllocateInfo allocateInfo = {};
  allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocateInfo.pNext = nullptr;

  allocateInfo.pSetLayouts = &layout;
  allocateInfo.descriptorPool = currentPool;
  allocateInfo.descriptorSetCount = 1;

  VkResult allocResult = vkAllocateDescriptorSets(window->getDevice(), &allocateInfo, set);
  bool needReallocate = false;

  switch(allocResult) {
    case VK_SUCCESS:
      return true;
    case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
      needReallocate = true;
      break;
    default:
      return false;
  }

  if(needReallocate) {
    currentPool = grabPool();
    usedPools.push_back(currentPool);
    allocResult = vkAllocateDescriptorSets(window->getDevice(), &allocateInfo, set);
    if(allocResult == VK_SUCCESS) {
      return true;
    }
  }

  return false;
}

void DescriptorAllocator::resetPools() {
  for(auto p : usedPools) {
    vkResetDescriptorPool(window->getDevice(), p, 0);
    freePools.push_back(p);
  }
  usedPools.clear();
  currentPool = VK_NULL_HANDLE;
}

}