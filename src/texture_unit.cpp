#include "texture_unit.h"
#include "texture.h"

namespace gl {


std::atomic<unsigned> TextureUnit::s_unit(1u);



TextureUnit::TextureUnit(): _unit(s_unit++) {
  // assert _unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
}

TextureUnit::~TextureUnit() {
}

void TextureUnit::add(Texture const& texture) {
  ActiveTextureBindguard guard(*this);
  GL_CALL(glBindTexture(texture.target(), texture.name()));
}

GLenum TextureUnit::unit() const {
  return _unit;
}




}
