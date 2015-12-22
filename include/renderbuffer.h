#ifndef UGLY_RENDERBUFFER_H
#define UGLY_RENDERBUFFER_H

#include "gl_type.h"
#include "generated_object.h"

namespace gl {

class Renderbuffer : public GeneratedObject<glGenRenderbuffers, glDeleteRenderbuffers> {
  public:
    explicit Renderbuffer(GLenum internal_format);
    explicit Renderbuffer(GLenum internal_format, GLsizei width, GLsizei height);

  public:
    void storage(GLsizei width, GLsizei height);

  private:
    GLenum _internal_format;

};

} // namespace gl

#endif
