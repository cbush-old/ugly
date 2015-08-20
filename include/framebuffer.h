#ifndef UGLY_FRAMEBUFFER_H
#define UGLY_FRAMEBUFFER_H

#include "gl_type.h"

namespace gl {

class Framebuffer {
  public:
    void bind(GLenum target);
    void unbind();

};

} // namespace gl

#endif
