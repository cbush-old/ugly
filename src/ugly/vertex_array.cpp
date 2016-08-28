#include "vertex_array.h"
#include "program.h"
#include "buffer.h"
#include "texture_unit.h"
#include "framebuffer.h"
#include "uniform.h"

namespace gl {


VertexArray::VertexArray() {}

VertexArray::VertexArray(GLenum mode)
  : _mode(mode)
  {}


void VertexArray::pointer(Buffer const& buffer, attrib const& attrib, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset) {
  BufferBindguard buffer_guard(GL_ARRAY_BUFFER, buffer);
  VertexArrayBindguard vertex_array_guard(*this);
  GL_CALL(glVertexAttribPointer(attrib.location(), size, type, normalized, stride, (void const*)offset));
}

void VertexArray::enable(attrib const& attrib) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glEnableVertexAttribArray(attrib.location()));
}

void VertexArray::disable(attrib const& attrib) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glDisableVertexAttribArray(attrib.location()));
}

GLenum VertexArray::mode() const {
  return _mode;
}

GLsizei VertexArray::count() const {
  return _count;
}

void VertexArray::set_mode(GLenum mode) {
  _mode = mode;
}

void VertexArray::set_count(GLsizei count) {
  _count = count;
}

std::vector<size_t> const& VertexArray::segments() const {
  return _segments;
}

void VertexArray::set_segments(std::vector<size_t> const& segments) {
  _segments = segments;
}

} // namespace gl
