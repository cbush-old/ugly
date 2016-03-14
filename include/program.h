#ifndef UGLY_PROGRAM_H
#define UGLY_PROGRAM_H

#include <memory>
#include <unordered_map>
#include <string>

#include "gl_type.h"
#include "shader.h"
#include "context.h"


namespace gl {

class untyped_uniform;
class attrib;

struct uniform_info {
  GLuint index;
  GLenum type;
  GLint size;
  std::string name;
};

struct Binary {
  std::vector<uint8_t> buffer;
  GLenum format;
  Binary() {}
  Binary(GLsizei size): buffer(size) {}
};


class Program {
  public:
    Program();

    template<typename... ShaderT>
    Program(Shader const&, ShaderT const&...);
  
  public:
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;
    ~Program();

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
    untyped_uniform operator[](const char* name) const;
    untyped_uniform operator[](std::string const& name) const;
    untyped_uniform uniform(const char* name) const;
    untyped_uniform uniform(std::string const& name) const;
    untyped_uniform uniform(GLint location) const;

  public: // uniform block, uniform buffer
    GLuint uniform_block_index(const char* name) const;
    GLuint uniform_block_index(std::string const& name) const;

  public:
    GLint attrib_location(const char* name) const;
    GLint attrib_location(std::string const& name) const;
    class attrib attrib(const char* name) const;
    class attrib attrib(std::string const& name) const;

  public:
    GLint get(GLenum) const;
    Binary binary() const;
    void binary(Binary const&);

  public:
    /**
     * @brief retrieve properties corresponding to a specified shader stage
     **/
    GLint stage(GLenum shader_type, GLenum param) const;
    bool validate() const;
    std::string info_log() const;

  public:
    /**
     * @brief specify a parameter
     **/
    void parameter(GLenum param, GLint value);
  
  private:
    void attach() {}

  private:
    GLuint _name;

};


template<typename... ShaderT>
inline Program::Program(Shader const& shader, ShaderT const&... shaders)
  : Program()
{
  attach(shader, shaders...);
  link();
}

template<typename ShaderT, typename... ShaderV>
inline void Program::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<Shader const&>(first));
  attach(the_rest...);
}



} // namespace gl

#endif
