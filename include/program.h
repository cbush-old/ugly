#ifndef OGL_PROGRAM_H
#define OGL_PROGRAM_H

#include "gl_type.h"
#include "shader.h"
#include "uniform.h"

namespace gl {

class Program : public IProgram {
  public:
    Program();

  public:
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    void attach(IShader const&) override;
    void detach(IShader const&) override;
    void link() override;

  private:
    GLint _name { 0 };

};

}

#endif
