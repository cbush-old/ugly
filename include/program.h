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

struct Binary {
  std::vector<uint8_t> buffer;
  GLenum format;
  Binary() {}
  Binary(GLsizei size): buffer(size) {}
};

class Program;

class ProgramRef {
  public:
    ProgramRef();

    template<typename... ShaderT>
    ProgramRef(Shader const&, ShaderT const&...);
  
  public:
    ProgramRef(ProgramRef const&);
    ProgramRef& operator=(ProgramRef const&);
    ~ProgramRef();

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
    std::shared_ptr<Program> _shared;

};


template<typename... ShaderT>
inline ProgramRef::ProgramRef(Shader const& shader, ShaderT const&... shaders)
  : ProgramRef()
{
  attach(shader, shaders...);
  link();
}

template<typename ShaderT, typename... ShaderV>
inline void ProgramRef::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<Shader const&>(first));
  attach(the_rest...);
}



using ProgramConstRef = ProgramRef const;



} // namespace gl

#endif
