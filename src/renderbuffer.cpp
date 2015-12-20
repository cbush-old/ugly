#include "renderbuffer.h"

namespace gl {

Renderbuffer::Renderbuffer(GLenum internal_format)
  : _internal_format(internal_format)
  {}

Renderbuffer::Renderbuffer(GLenum internal_format, GLsizei width, GLsizei height)
  : Renderbuffer(internal_format) {
  storage(width, height);
}

void Renderbuffer::storage(GLsizei width, GLsizei height) {
  RenderbufferBindguard guard(GL_RENDERBUFFER, name());
  GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, _internal_format, width, height));
}


} // namespace gl
