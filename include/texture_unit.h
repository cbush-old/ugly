#ifndef UGLY_TEXTURE_UNIT_H
#define UGLY_TEXTURE_UNIT_H


#include "gl_type.h"


namespace gl {


class TextureUnit {
  public:
    TextureUnit();
    ~TextureUnit();
  
  public:
    template<typename TextureType>
    void enable(TextureType const& texture) const;

};



}


#endif
