#ifndef UGLY_TEXTURE_UNIT_H
#define UGLY_TEXTURE_UNIT_H


#include "gl_type.h"

namespace gl {

class Texture;

class TextureUnit {
  private:
    static std::atomic<unsigned> s_unit;

  public:
    TextureUnit();
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
