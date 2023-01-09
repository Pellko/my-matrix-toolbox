#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VK_MEMORY_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VK_MEMORY_H_

#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"

namespace mymatrixtoolbox {
namespace vkmemory {

struct AllocatedBuffer {
  VkBuffer buffer;
  VmaAllocation allocation;
};

}
}

#endif