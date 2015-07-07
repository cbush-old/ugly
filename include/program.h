#ifndef OGL_PROGRAM_H
#define OGL_PROGRAM_H

#include "gl_type.h"
#include "shader.h"

namespace ogl {

class Program {  
  public:
    Program();
    ~Program();

  public:
    void attach(Shader const&) const;
    void detach(Shader const&) const;
    void link() const;
    void bind() const;
    void enable(attribute const) const;
    void disable(attribute const) const;
    void unbind() const;

  public:
    uniform get_uniform(const char*) const;
    attribute get_attrib(const char*) const;
    void set_uniform(uniform, float const) const;
    void set_uniform(uniform, float const, float const) const;
    void set_uniform(uniform, float const, float const, float const) const;
    void set_uniform(uniform, float const, float const, float const, float const) const;
    void set_matrix(uniform, GLsizei const, GLboolean const, GLfloat const *) const;

  private:
    GLint _id { 0 };

};

}

#endif
