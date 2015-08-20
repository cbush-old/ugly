#ifndef UGLY_BUFFER_H
#define UGLY_BUFFER_H

#include "gl_type.h"

namespace gl {

class Buffer : public GeneratedObject<glGenBuffers, glDeleteBuffers>, ContextAssociatedObject {
  public:
    Buffer(IContext&);
    Buffer(IContext&, GLuint);

  public:
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;
    ~Buffer();

};

} // namespace gl

#endif
