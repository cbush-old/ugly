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


typedef Shader_type<GL_FRAGMENT_SHADER> Fragment_shader;
typedef Shader_type<GL_VERTEX_SHADER> Vertex_shader;
typedef Shader_type<GL_TESS_CONTROL_SHADER> Tess_control_shader;
typedef Shader_type<GL_TESS_EVALUATION_SHADER> Tess_evaluation_shader;
typedef Shader_type<GL_GEOMETRY_SHADER> Geometry_shader;


}

#endif

