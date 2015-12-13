#ifndef UGLY_VERTEX_ARRAY_H
#define UGLY_VERTEX_ARRAY_H

#include "gl_type.h"
#include "generated_object.h"

#include <string>

namespace gl {


class Program;
class Buffer;
class TextureUnit;

class attrib {
  public:
    attrib(Program const&, const char* name);
    attrib(Program const&, std::string const& name);
    attrib(Program const&, GLint location);

  public:
    GLint location() const;

  protected:
    Program const& _program;
    GLint _location { -1 };

};


class VertexArray : public GeneratedObject<glGenVertexArrays, glDeleteVertexArrays> {
  public:
    void enable(attrib const&);
    void disable(attrib const&);

  public:
    void pointer(Buffer& buffer, attrib const& attrib, GLint size, GLenum type, bool normalized, GLsizei stride, size_t offset);
    void draw(GLenum mode, GLsizei count, GLsizei first = 0);
};

} // namespace gl

#endif
