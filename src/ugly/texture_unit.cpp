#include "texture_unit.h"
#include "texture.h"

using namespace gl;

namespace {

std::atomic<unsigned> s_unit(1u);

using lock = std::lock_guard<std::mutex>;
std::mutex _lock;
std::vector<unsigned> recycle_bin;

unsigned next_unit() {
  lock guard(_lock);
  if (recycle_bin.size()) {
    auto unit = recycle_bin.back();
    recycle_bin.pop_back();
    return unit;
  }
  return s_unit++;
}

void release_unit(unsigned unit) {
  lock guard(_lock);
  recycle_bin.push_back(unit);
}


}


TextureUnit::TextureUnit(): _unit(next_unit()) {
  // assert _unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
}

TextureUnit::~TextureUnit() {
  release_unit(_unit);
}

void TextureUnit::add(Texture const& texture) {
  ActiveTextureBindguard guard(*this);
  GL_CALL(glBindTexture(texture.target(), texture.name()));
}

GLenum TextureUnit::unit() const {
  return _unit;
}

