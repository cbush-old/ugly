#ifndef UGLY_FRAMEBUFFER_H
#define UGLY_FRAMEBUFFER_H

#include "gl_type.h"
#include "generated_object.h"

namespace gl {


class Texture1D;
class Texture2D;
class Texture3D;
class Cubemap;
class Renderbuffer;


class Framebuffer : public GeneratedObject<glGenFramebuffers, glDeleteFramebuffers> {
  public:
    void texture(GLenum attachment, Texture1D const& texture, int level = 0);
    void texture(GLenum attachment, Texture2D const& texture, int level = 0);
    void texture(GLenum attachment, Cubemap const& texture, int level);
    void texture(GLenum attachment, Texture3D const& texture, int level, int layer);
  
  public:
    void renderbuffer(GLenum attachment, Renderbuffer const& renderbuffer);
  
  public:
    GLenum status() const;
    const char* status_str() const;

};

} // namespace gl

#endif
