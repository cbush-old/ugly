#ifndef UGLY_PROGRAM_H
#define UGLY_PROGRAM_H

#include <memory>
#include <unordered_map>
#include <string>

#include "gl_type.h"
#include "shader.h"
#include "context.h"


namespace gl {

struct uniform_info {
  GLuint index;
  GLenum type;
  GLint size;
  std::string name;
};

class Program;

using ProgramRef = std::shared_ptr<Program>;
using ProgramConstRef = std::shared_ptr<Program const>;

class Program {
  public:
    struct Binary {
      std::vector<uint8_t> buffer;
      GLenum format;
      Binary() {}
      Binary(GLsizei size): buffer(size) {}
    };

  private:
    Program();

  public: // convenience
    /**
     * @brief construct a program with attached shaders
     **/
    template<typename... ShaderT>
    static ProgramRef create(Shader const&, ShaderT const&...);

    static ProgramRef create();

  public:
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    void attach(Shader const&);
    void detach(Shader const&);
    void link();

  public:
    template<typename ShaderT, typename... ShaderV>
    void attach(ShaderT const&, ShaderV const&...);

  public:
    GLuint name() const;

  public: // uniforms
    GLint uniform_location(const char* name) const;
    GLint uniform_location(std::string const& name) const;
    uniform_info active_uniform(GLuint index) const;

  public:
    GLint attrib_location(const char* name) const;
    GLint attrib_location(std::string const& name) const;

  public:
    GLint get(GLenum) const;
    Binary binary() const;
    void binary(Binary const&);

    /**
     * @brief retrieve properties corresponding to a specified shader stage
     **/
    GLint stage(GLenum shader_type, GLenum param) const;

    /**
     * @brief specify a parameter
     **/
    void parameter(GLenum param, GLint value);

    bool validate() const;

    std::string info_log() const;

  private:
    void attach() {}

  private:
    class Program_impl* _impl;
  
};

template<typename... ShaderT>
inline ProgramRef Program::create(Shader const& shader, ShaderT const&... shaders) {
  ProgramRef program (new Program());
  program->attach(shader, shaders...);
  program->link();
  return program;
}

template<typename ShaderT, typename... ShaderV>
inline void Program::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<Shader const&>(first));
  attach(the_rest...);
}



} // namespace gl

#endif
