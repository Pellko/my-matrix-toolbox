#ifndef _MY_MATRIX_TOOLBOX_GRAPHICS_FONT_H_
#define _MY_MATRIX_TOOLBOX_GRAPHICS_FONT_H_

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "Memory.hh"
#include "Window.hh"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace mymatrixtoolbox {

struct Character {
  glm::vec2 offset;
  glm::vec2 size;
  glm::ivec2 pixelSize;
  glm::ivec2 bearing;
  unsigned int advance;
};

class Font {
 public:
  Font();
  ~Font();

  void init(std::shared_ptr<Window> window);
  void terminate();
  
  VkImageView getImageView() {
    return imageView;
  }

  Character& getCharacter(char c) {
    return characters[c];
  }

 private:
  FT_Library ft;
  FT_Face face;
  std::vector<Character> characters;
  vkmemory::AllocatedImage image;
  VkImageView imageView;
};

}

#endif
