#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_VK_UTIL_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_VK_UTIL_H_

#include <vulkan/vulkan.hpp>
#include <deque>
#include <functional>
#include "Memory.hh"

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
VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo();
VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(VkPrimitiveTopology topology);
VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode polygonMode);
VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo();
VkPipelineColorBlendAttachmentState colorBlendAttachmentState();
VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo();
VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0);
VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
VkSubmitInfo submitInfo(VkCommandBuffer* cmd);
VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags flags, VkExtent3D extent);
VkImageViewCreateInfo imageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

class PipelineBuilder {
 public:
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineLayout pipelineLayout;

	VkPipeline build(VkDevice device, VkRenderPass renderPass);
};

struct DeletionQueue {
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)();
		}
		deletors.clear();
	}
};

struct VertexInputDescription {
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;
  VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct UploadContext {
	VkFence fence;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
};

}
}

#endif