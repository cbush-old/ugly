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
    Program(ShaderT const&...);



    /**
     * @brief construct a program with attached shaders, then use the program with context
     **/
    Program(IContext const&, VertexShader const&, FragmentShader const&);


  public:
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    void attach(IShader const&) override;
    void detach(IShader const&) override;
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
inline Program::Program(ShaderT const&... shaders): Program() {
  GL_VALIDATE(Program, _name);
  attach(shaders...);
  link();
}

template<typename ShaderT, typename... ShaderV>
inline void Program::attach(ShaderT const& first, ShaderV const&... the_rest) {
  attach(static_cast<IShader const&>(first));
  attach(the_rest...);
}


}

#endif
