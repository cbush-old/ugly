#ifndef OGL_PROGRAM_H
#define OGL_PROGRAM_H

#include "gl_type.h"
#include "shader.h"
#include "uniform.h"

namespace gl {

class Program {  
  public:
    Program();
    ~Program();

  public:
    /**
     * @brief Attach a Shader to the Program.
     * @param shader  The Shader to attach.
     **/
    void attach(Shader const& shader) const;

    /**
     * @brief Detach the given Shader from the Program.
     * @param shader  The Shader to detach.
     **/
    void detach(Shader const& shader) const;

    /**
     * @brief Link the Program.
     * 
     * Call this after all the shaders are attached.
     **/
    void link() const;

    /**
     * @brief Bind the Program to the current Context.
     **/
    void bind() const;

    /**
     * @brief Unbind the Program from the current Context. 
     **/
    void unbind() const;

    void enable(attribute const) const;
    void disable(attribute const) const;

  public:
    /*
    uniform get_uniform(const char*) const;
    attribute get_attrib(const char*) const;
    void set_uniform(uniform, float const) const;
    void set_uniform(uniform, float const, float const) const;
    void set_uniform(uniform, float const, float const, float const) const;
    void set_uniform(uniform, float const, float const, float const, float const) const;
    void set_matrix(uniform, GLsizei const, GLboolean const, GLfloat const *) const;
    */
  private:
    GLint _name { 0 };

};

}

#endif
