#include "vertex_array.h"
#include "program.h"
#include "buffer.h"
#include "texture_unit.h"
#include "framebuffer.h"
#include "uniform.h"

namespace gl {


VertexArray::VertexArray(ProgramConstRef program)
  : _program(program)
  {}

VertexArray::VertexArray(ProgramConstRef program, GLenum mode)
  : _program(program)
  , _mode(mode)
  {}


ProgramConstRef VertexArray::program() const {
  return _program;
}


void VertexArray::pointer(Buffer& buffer, attrib const& attrib, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset) {
  BufferBindguard buffer_guard(GL_ARRAY_BUFFER, buffer);
  VertexArrayBindguard vertex_array_guard(*this);
  GL_CALL(glVertexAttribPointer(attrib.location(), size, type, normalized, stride, (void const*)offset));
}

void VertexArray::pointer(Buffer& buffer, const char* attrib_name, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset) {
  BufferBindguard buffer_guard(GL_ARRAY_BUFFER, buffer);
  VertexArrayBindguard vertex_array_guard(*this);
  GL_CALL(glVertexAttribPointer(_program.attrib_location(attrib_name), size, type, normalized, stride, (void const*)offset));
}

void VertexArray::pointer(Buffer& buffer, std::string const& attrib_name, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset) {
  pointer(buffer, attrib_name.c_str(), size, type, normalized, stride, offset);
}

void VertexArray::enable(attrib const& attrib) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glEnableVertexAttribArray(attrib.location()));
}

void VertexArray::enable(const char* attrib_name) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glEnableVertexAttribArray(_program.attrib_location(attrib_name)));
}

void VertexArray::enable(std::string const& attrib_name) {
  enable(attrib_name.c_str());
}

void VertexArray::disable(attrib const& attrib) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glDisableVertexAttribArray(attrib.location()));
}

void VertexArray::disable(const char* attrib_name) {
  VertexArrayBindguard guard(*this);
  GL_CALL(glDisableVertexAttribArray(_program.attrib_location(attrib_name)));
}

void VertexArray::disable(std::string const& attrib_name) {
  disable(attrib_name.c_str());
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


} // namespace gl
