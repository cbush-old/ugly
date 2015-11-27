#ifndef UGLY_TEXTURE_H
#define UGLY_TEXTURE_H

#include "gl_type.h"
#include "generated_object.h"

namespace gl {

class Texture : public GeneratedObject<glGenTextures, glDeleteTextures> {
  public:
    Texture();
    Texture(GLuint name);

  public:
    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;
    virtual ~Texture() =0;

};



} // namespace gl

#endif
