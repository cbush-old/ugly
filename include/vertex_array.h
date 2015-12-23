#ifndef UGLY_VERTEX_ARRAY_H
#define UGLY_VERTEX_ARRAY_H

#include "gl_type.h"
#include "generated_object.h"
#include "program.h"

#include <string>

namespace gl {


class Program;
class Buffer;
class Framebuffer;
class attrib;


class VertexArray : public GeneratedObject<glGenVertexArrays, glDeleteVertexArrays> {
  public:
    VertexArray(ProgramConstRef);

  public:
    void enable(attrib const&);
    void enable(const char* attrib_name);
    void enable(std::string const& attrib_name);
    void disable(attrib const&);
    void disable(const char* attrib_name);
    void disable(std::string const& attrib_name);

  public:
    void pointer(Buffer& buffer, attrib const& attrib, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset);
    void pointer(Buffer& buffer, const char* attrib_name, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset);
    void pointer(Buffer& buffer, std::string const& attrib_name, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset);
    void draw(GLenum mode, GLsizei count, GLsizei first = 0);
    void draw(Framebuffer& framebuffer, GLenum mode, GLsizei count, GLsizei first = 0);
  
  public:
    ProgramConstRef program() const;

  private:
    ProgramConstRef _program;

};

} // namespace gl

#endif
