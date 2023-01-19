#include "Font.hh"
#include "src/graphics/VkUtil.hh"
#include <iostream>

namespace mymatrixtoolbox {

Font::Font() {}
Font::~Font() {}

void Font::init(std::shared_ptr<Window> window) {
  if(FT_Init_FreeType(&ft)) {
    std::cout << "[FreeType]: Could not init FreeType" << std::endl;
    return;
  }

  if(FT_New_Face(ft, "assets/fonts/Roboto-Regular.ttf", 0, &face)) {
    std::cout << "[FreeType]: Failed to load font" << std::endl; 
    return;
  }

  characters.resize(128);
  FT_Set_Pixel_Sizes(face, 0, 48);

  const uint32_t ATLAS_WIDTH = 2048;

  uint32_t spriteWidth = 0;
  uint32_t spriteHeight = 0;

  // Calculate the maximum size of a glyph
  for(uint8_t c=0;c<128;c++) {
    // Load character
    if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "[FreeType]: Failed to load glyph" << std::endl;
      continue;
    }

    // Get character size
    uint32_t width = face->glyph->bitmap.width;
    uint32_t height = face->glyph->bitmap.rows;

    spriteWidth = std::max(spriteWidth, width);
    spriteHeight = std::max(spriteHeight, height);
  }

  // Calculate number of columns and rows
  uint32_t numCols = floor((float) ATLAS_WIDTH / (float) spriteWidth);
  uint32_t numRows = ceil((float) 128 / (float) numCols);

  const uint32_t atlasWidth = spriteWidth * numCols;
  const uint32_t atlasHeight = spriteHeight * numRows;
  const size_t bufferSize = atlasWidth * atlasHeight * 4;
  std::vector<uint8_t> buffer(bufferSize);

  for(int y=0;y<numRows;y++) {
    for(int x=0;x<numCols;x++) {
      // Load character
      uint8_t glyph = x + y * numCols;
      if(glyph >= 128) continue;

      if(FT_Load_Char(face, glyph, FT_LOAD_RENDER)) {
        std::cout << "[FreeType]: Failed to load glyph" << std::endl;
        continue;
      }

      uint32_t targetX = x * spriteWidth;
      uint32_t targetY = y * spriteHeight;
      uint32_t glyphWidth = face->glyph->bitmap.width;
      uint32_t glyphHeight = face->glyph->bitmap.rows;

      for(int i=0;i<spriteHeight;i++) {
        for(int j=0;j<spriteWidth;j++) {
          int srcOffset = j + i * spriteWidth;          
          int dstOffset = (j + targetX) + (i + targetY) * spriteWidth;

          if(srcOffset >= glyphWidth * glyphHeight) {
            buffer[dstOffset + 0] = 0xFF;
            buffer[dstOffset + 1] = 0xFF;
            buffer[dstOffset + 1] = 0xFF;
            buffer[dstOffset + 1] = 0x00;
          } else {
            auto value = face->glyph->bitmap.buffer + srcOffset;
            buffer[dstOffset + 0] = 0xFF;
            buffer[dstOffset + 1] = 0xFF;
            buffer[dstOffset + 1] = 0xFF;
            buffer[dstOffset + 1] = *value;
          }
        }
      }

      // Store reference to glyph
      characters[glyph] = Character{
        .offset = { (float) targetX / (float) atlasWidth, (float) targetY / (float) atlasHeight },
        .size = { (float) face->glyph->metrics.width / (float) atlasWidth, (float) face->glyph->metrics.height / (float) atlasHeight },
        .advance = static_cast<unsigned int>(face->glyph->advance.x),
        .bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top },
      };
    }
  }

  // Create a staging buffer
  VkDeviceSize deviceSize = bufferSize;
  VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;

  vkmemory::AllocatedBuffer stagingBuffer = window->createBuffer(deviceSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
  
  void* data;
  vmaMapMemory(window->getAllocator(), stagingBuffer.allocation, &data);
  memcpy(data, buffer.data(), bufferSize);
  vmaUnmapMemory(window->getAllocator(), stagingBuffer.allocation);

  // Create image
  VkExtent3D imageExtent;
  imageExtent.width = atlasWidth;
  imageExtent.height = atlasHeight;
  imageExtent.depth = 1;

  VkImageCreateInfo imageInfo = vkutil::imageCreateInfo(imageFormat, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);

  VmaAllocationCreateInfo imageAllocInfo = {};
  imageAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  vmaCreateImage(window->getAllocator(), &imageInfo, &imageAllocInfo, &image.image, &image.allocation, nullptr);

  window->immediateSubmit([=](VkCommandBuffer cmd) {
    VkImageSubresourceRange range;
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.baseArrayLayer = 0;
		range.layerCount = 1;

    VkImageMemoryBarrier imageBarrierToTransfer = {};
    imageBarrierToTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageBarrierToTransfer.pNext = nullptr;
    imageBarrierToTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageBarrierToTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageBarrierToTransfer.image = image.image;
    imageBarrierToTransfer.subresourceRange = range;
    imageBarrierToTransfer.srcAccessMask = 0;
    imageBarrierToTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrierToTransfer);

    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;
    
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageExtent = imageExtent;

    vkCmdCopyBufferToImage(cmd, stagingBuffer.buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    VkImageMemoryBarrier imageBarrierToReadable = imageBarrierToTransfer;
    imageBarrierToReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageBarrierToReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageBarrierToReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageBarrierToReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrierToReadable);
  });

  window->getDeletionQueue().push_function([=]() {
    vmaDestroyImage(window->getAllocator(), image.image, image.allocation);
  });
  vmaDestroyBuffer(window->getAllocator(), stagingBuffer.buffer, stagingBuffer.allocation);

  // Create image view
  VkImageViewCreateInfo imageViewInfo = vkutil::imageViewCreateInfo(VK_FORMAT_R8G8B8A8_SRGB, image.image, VK_IMAGE_ASPECT_COLOR_BIT);
  VK_CHECK(vkCreateImageView(window->getDevice(), &imageViewInfo, nullptr, &imageView));

  window->getDeletionQueue().push_function([=]() {
    vkDestroyImageView(window->getDevice(), imageView, nullptr);
  });
  
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

}