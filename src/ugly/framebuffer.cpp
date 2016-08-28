#include "framebuffer.h"
#include "texture.h"
#include "renderbuffer.h"
#include "vertex_array.h"
#include "program.h"

namespace gl {



#define FRAMEBUFFER_TEXTURE_IMPL(ND, ...) \
  FramebufferBindguard guard(GL_FRAMEBUFFER, *this); \
  GL_CALL(glFramebufferTexture##ND ( \
    GL_FRAMEBUFFER, \
    attachment, \
    texture.target(), \
    texture.name(), \
    __VA_ARGS__ \
  ));

void Framebuffer::texture(GLenum attachment, Texture1D const& texture, int level) {
  FRAMEBUFFER_TEXTURE_IMPL(1D, level);
}

void Framebuffer::texture(GLenum attachment, Texture2D const& texture, int level) {
  FRAMEBUFFER_TEXTURE_IMPL(2D, level);
}

void Framebuffer::texture(GLenum attachment, Texture3D const& texture, int level, int layer) {
  FRAMEBUFFER_TEXTURE_IMPL(3D, level, layer);
}

void Framebuffer::texture(GLenum attachment, Cubemap const& texture, int level) {
  FRAMEBUFFER_TEXTURE_IMPL(2D, level);
}

#undef FRAMEBUFFER_TEXTURE_IMPL

void Framebuffer::renderbuffer(GLenum attachment, Renderbuffer const& renderbuffer) {
  FramebufferBindguard guard(GL_FRAMEBUFFER, *this);
  GL_CALL(glFramebufferRenderbuffer(
    GL_FRAMEBUFFER,
    attachment,
    GL_RENDERBUFFER,
    renderbuffer.name()
  ));
}


GLenum Framebuffer::status() const {
  FramebufferBindguard guard(GL_FRAMEBUFFER, *this);
  GLenum rv;
  GL_CALL(rv = glCheckFramebufferStatus(GL_FRAMEBUFFER));
  return rv;
}

const char* Framebuffer::status_str() const {
  GLenum status (this->status());
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE: return "complete";
    case GL_FRAMEBUFFER_UNDEFINED: return "undefined";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "incomplete attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "incomplete missing attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "incomplete draw buffer";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "incomplete read buffer";
    case GL_FRAMEBUFFER_UNSUPPORTED: return "unsupported";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "incomplete multisample";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "incomplete layer targets";
    default: return "unknown";
  }
}

void BasicFramebuffer::viewport(float x, float y, float w, float h) {
  _viewport = Viewport(x, y, w, h);
}

void BasicFramebuffer::viewport(Viewport viewport) {
  _viewport = viewport;
}

Viewport const& BasicFramebuffer::viewport() const {
  return _viewport;
}

void BasicFramebuffer::clear_color(float r, float g, float b, float a /* = 1.f */) {
  _clear_color = color(r, g, b, a);
}

void BasicFramebuffer::clear_color(color c) {
  _clear_color = c;
}

void BasicFramebuffer::draw(Program const& program, VertexArray const& vao) {
  auto const& segments = vao.segments();
  if (segments.empty()) {
    GL_ASSERT(vao.count(), "draw called with 0-count vertex array %p", &vao);
    draw(program, vao, vao.mode(), vao.count());
  } else {
    GLsizei offset = 0;
    for (size_t segment_size : segments) {
      draw(program, vao, vao.mode(), (GLsizei)segment_size, offset);
      offset += (GLsizei)segment_size;
    }
  }
}

void Framebuffer::clear(GLenum mask) {
  FramebufferBindguard guard(GL_FRAMEBUFFER, *this);
  GL_CALL(glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a));
  GL_CALL(glClear(mask));
}

void Framebuffer::draw(Program const& program, VertexArray const& vao, GLenum mode, size_t count, size_t first /* = 0 */) {
  VertexArrayBindguard guard(vao);
  ProgramBindguard program_guard(program);
  FramebufferBindguard fb_guard(GL_FRAMEBUFFER, *this);
  GL_CALL(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
  GL_CALL(glDrawArrays(mode, (GLsizei)first, (GLsizei)count));
}

void Framebuffer::draw_instanced(Program const& program, VertexArray const& vao, size_t instance_count, GLenum mode, size_t count, size_t first /* = 0 */) {
  VertexArrayBindguard guard(vao);
  ProgramBindguard program_guard(program);
  FramebufferBindguard fb_guard(GL_FRAMEBUFFER, *this);
  GL_CALL(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
  GL_CALL(glDrawArraysInstanced(mode, (GLsizei)first, (GLsizei)count, (GLsizei)instance_count));
}

void BasicFramebuffer::draw_buffer(GLenum buffer) {
  GL_CALL(glDrawBuffer(buffer));
}

void Framebuffer::draw_buffer(GLenum buffer) {
  FramebufferBindguard guard(GL_FRAMEBUFFER, *this);
  BasicFramebuffer::draw_buffer(buffer);
}

bool Framebuffer::is_complete() const {
  return status() == GL_FRAMEBUFFER_COMPLETE;
}



} // namespace gl
