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
    VertexArray(ProgramConstRef, GLenum mode);

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
  
  public: // Optionally store count and mode params for use with drawing.
    GLsizei count() const;
    void set_count(GLsizei);
  
    GLenum mode() const;
    void set_mode(GLenum);
  
    std::vector<size_t> const& segments() const;
    void set_segments(std::vector<size_t> const&);

  public:
    ProgramConstRef program() const;

  private:
    ProgramConstRef _program;
    GLenum _mode { GL_POINTS };
    GLsizei _count { 0 };
    std::vector<size_t> _segments;

};


} // namespace gl

#endif
