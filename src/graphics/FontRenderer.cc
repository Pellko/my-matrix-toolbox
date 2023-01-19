#include "FontRenderer.hh"

namespace mymatrixtoolbox {

FontRenderer::FontRenderer(std::shared_ptr<Window> window, int zIndex) : Renderable(zIndex), window(window) {}
FontRenderer::~FontRenderer() {}

void FontRenderer::init() {
  font.init(window);
}

void FontRenderer::render() {}

void FontRenderer::terminate() {}

} 