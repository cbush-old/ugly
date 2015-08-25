#ifndef OGL_PROGRAM_H
#define OGL_PROGRAM_H

#include "gl_type.h"
#include "shader.h"
#include "uniform.h"

namespace gl {

class Program {
  public:
    Program();

  public: // convenience constructors
    /**
     * @brief construct a program with attached shaders
     **/
    template<typename... ShaderT>
    Program(Shader const&, ShaderT const&...);

    /**
     * @brief construct a program with attached shaders, then use the program with context
     **/
    template<typename... ShaderT>
    Program(BaseContext const&, Shader const&, ShaderT const&...);

  public:
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    void attach(Shader const&);
    void detach(Shader const&);
    void link();
    void use(BaseContext const&);

  public:
    template<typename ShaderT, typename... ShaderV>
    void attach(ShaderT const&, ShaderV const&...);

  public:
    GLuint name() const;

  public:
    GLint uniform_location(const char* name) const;
    GLint uniform_location(std::string const& name) const;

    template<typename... T>
    gl::uniform<T...> uniform(const char* name) const {
      return uniform_location(name);
    }

    template<typename... T>
    gl::uniform<T...> uniform(std::string const& name) const {
      return uniform_location(name.c_str());
    }

  private:
    void attach() {}

  private:
    class Program_impl* _impl;

};

template<typename... ShaderT>
inline Program::Program(Shader const& shader, ShaderT const&... shaders): Program() {
  attach(shader, shaders...);
  link();
}


template<typename... ShaderT>
inline Program::Program(BaseContext const& context, Shader const& shader, ShaderT const&... shaders): Program(shader, shaders...) {
  use(context);
}

template<typename ShaderT, typename... ShaderV>
inline void Program::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<Shader const&>(first));
  attach(the_rest...);
}



} // namespace gl

#endif
