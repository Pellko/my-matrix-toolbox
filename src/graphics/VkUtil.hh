#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VK_UTIL_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VK_UTIL_H_

#include <vulkan/vulkan.hpp>

namespace mymatrixtoolbox {
namespace vkutil {

#define VK_CHECK(x)                                                 \
	do                                                                \
	{                                                                 \
		VkResult err = x;                                               \
		if (err)                                                        \
		{                                                               \
			std::cout <<"Detected Vulkan error: " << err << std::endl;    \
			abort();                                                      \
		}                                                               \
	} while (0)

VkCommandPoolCreateInfo commandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level);

}
}

#endif