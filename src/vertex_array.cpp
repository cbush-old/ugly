#include "vertex_array.h"
#include "program.h"
#include "buffer.h"
#include "texture_unit.h"
#include "framebuffer.h"

namespace gl {


attrib::attrib(ProgramConstRef program, const char* name)
  : _program(program)
  , _location(program->attrib_location(name))
  {}

attrib::attrib(ProgramConstRef program, std::string const& name)
  : _program(program)
  , _location(program->attrib_location(name))
  {}

attrib::attrib(ProgramConstRef program, GLint location)
  : _program(program)
  , _location(location)
  {}


GLint attrib::location() const {
  return _location;
}



VertexArray::VertexArray(ProgramConstRef program)
  : _program(program)
  {}


ProgramConstRef VertexArray::program() const {
  return _program;
}


void VertexArray::pointer(Buffer& buffer, attrib const& attrib, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset) {
  BufferBindguard buffer_guard(GL_ARRAY_BUFFER, buffer.name());
  VertexArrayBindguard vertex_array_guard(name());
  GL_CALL(glVertexAttribPointer(attrib.location(), size, type, normalized, stride, (void const*)offset));
}


void VertexArray::enable(attrib const& attrib) {
  VertexArrayBindguard guard(name());
  GL_CALL(glEnableVertexAttribArray(attrib.location()));
}


void VertexArray::disable(attrib const& attrib) {
  VertexArrayBindguard guard(name());
  GL_CALL(glDisableVertexAttribArray(attrib.location()));
}




} // namespace gl
