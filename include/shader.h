#ifndef SHADER_H
#define SHADER_H

#include "gl_type.h"
#include <string>

namespace gl {


class Shader : public IShader {
  public:
    Shader();

  public:
    virtual ~Shader() =0;
    Shader& operator=(Shader const&) = delete;
    Shader(Shader const&) = delete;

  public:
    void set_source(std::string const& source) override;
    void set_source(std::vector<std::string> const& sources) override;
    std::string get_source() const override;

  public:
    GLuint name() const override;
    GLenum type() const override;
    bool deleted() const override;
    bool compiled() const override;
    unsigned source_length() const override;

  public:
    void compile() override;

  protected:
    void load(std::string const& path);

  protected:
    GLenum _name { 0 };

};

template<GLenum ShaderType>
class Shader_type : public Shader {
  public:
    Shader_type();

    /**
     * @brief construct the Shader, load and compile the source at path
     **/
    Shader_type(std::string const& path);
    Shader_type(std::vector<std::string> const& paths);

  public:
    ~Shader_type() override {}

};


typedef Shader_type<GL_FRAGMENT_SHADER> FragmentShader;
typedef Shader_type<GL_VERTEX_SHADER> VertexShader;
typedef Shader_type<GL_TESS_CONTROL_SHADER> TessControlShader;
typedef Shader_type<GL_TESS_EVALUATION_SHADER> TessEvaluationShader;
typedef Shader_type<GL_GEOMETRY_SHADER> GeometryShader;


}

#endif

