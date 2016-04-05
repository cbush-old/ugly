#ifndef UGLY_TEXTURE_UNIT_H
#define UGLY_TEXTURE_UNIT_H


#include "gl_type.h"

namespace gl {

class Texture;

class TextureUnit {
  public:
    TextureUnit();

    template<typename TextureType>
    TextureUnit(TextureType const& texture)
      : TextureUnit() {
      add(texture);
    }

    template<typename TextureType, typename... MoreTextures>
    TextureUnit(TextureType const& texture, MoreTextures const&... args)
      : TextureUnit(args...) {
      add(texture);
    }

    ~TextureUnit();
  
  public:
    void add(Texture const& texture);
  
  public:
    unsigned unit() const;
  
  private:
    unsigned _unit;

};



}


#endif
