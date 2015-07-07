#ifndef SHADER_H
#define SHADER_H

#include "gl_type.h"

namespace ogl {

class Shader {
  protected:
    Shader(GLenum type);

  public:
    virtual ~Shader();

  public:
    void load(const char*);
    void compile() const;

  public:
    inline GLuint get_id() const {
      return _id;
    };

  protected:
    GLenum _id { 0 };

};

template<GLenum ShaderType>
class Shader_type : public Shader {
  public:
    Shader_type():
      Shader(ShaderType) {}

    ~Shader_type() override {}

};


using Fragment_shader = Shader_type<GL_FRAGMENT_SHADER>;
using Vertex_shader = Shader_type<GL_VERTEX_SHADER>;
using Tess_control_shader = Shader_type<GL_TESS_CONTROL_SHADER>;
using Tess_evaluation_shader = Shader_type<GL_TESS_EVALUATION_SHADER>;
using Geometry_shader = Shader_type<GL_GEOMETRY_SHADER>;

}

#endif

