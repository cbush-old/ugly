#ifndef OGL_PROGRAM_H
#define OGL_PROGRAM_H

#include "gl_type.h"
#include "shader.h"
#include "uniform.h"

namespace gl {

class Program : public IProgram {
  public:
    Program();

  public: // convenience constructors
    template<typename... ShaderT>
    Program(Shader const&, ShaderT const&...);

    /**
     * @brief construct a program with attached shaders, then use the program with context
     **/
    template<typename... ShaderT>
    Program(IContext const&, Shader const&, ShaderT const&...);

  public:
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    void attach(Shader const&) override;
    void detach(Shader const&) override;
    void link() override;
    void use(IContext const&) override;

  public:
    template<typename ShaderT, typename... ShaderV>
    void attach(ShaderT const&, ShaderV const&...);

  private:
    void attach() {}

  private:
    GLint _name { 0 };

};

template<typename... ShaderT>
inline Program::Program(Shader const& shader, ShaderT const&... shaders): Program() {
  GL_VALIDATE(Program, _name);
  attach(shader, shaders...);
  link();
}


template<typename... ShaderT>
inline Program::Program(IContext const& context, Shader const& shader, ShaderT const&... shaders): Program(shader, shaders...) {
  use(context);
}

template<typename ShaderT, typename... ShaderV>
inline void Program::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<Shader const&>(first));
  attach(the_rest...);
}


}

#endif
