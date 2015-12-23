#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include "program.h"

#include <vector>
#include <string>

namespace gl {


namespace detail {

class basic_uniform {
  protected:
    basic_uniform(ProgramConstRef program, const char* name);
    basic_uniform(ProgramConstRef program, std::string const& name);
    basic_uniform(ProgramConstRef program, GLint location);
    ~basic_uniform();

  public:
    GLint location() const;

  protected:
    ProgramConstRef _program;
    GLint _location { -1 };
};

template<typename... T>
class uniform : public basic_uniform {
  using vec_t = vec<T...>;
  
  public:
    uniform(ProgramConstRef program, const char* name);
    uniform(ProgramConstRef program, std::string const& name);
    uniform(ProgramConstRef program, GLint location);
    ~uniform();

  public:
    void set(T... values);
    void set(vec_t const& vec);
    vec_t get() const;

};


template<unsigned N, unsigned M = N>
class uniform_matrix : public basic_uniform {
  public:
    uniform_matrix(ProgramConstRef program, const char* name, GLsizei count = 1);
    uniform_matrix(ProgramConstRef program, std::string const& name, GLsizei count = 1);
    uniform_matrix(ProgramConstRef program, GLint location, GLsizei count = 1);
    ~uniform_matrix();

  public:
    void set(GLfloat const*, bool transpose = false);
  
  private:
    GLsizei _count;

};


}



template<typename T>
using uniform = detail::uniform<T>;

template<typename T>
using uniform2 = detail::uniform<T, T>;

template<typename T>
using uniform3 = detail::uniform<T, T, T>;

template<typename T>
using uniform4 = detail::uniform<T, T, T, T>;

using uniform_mat2 = detail::uniform_matrix<2>;
using uniform_mat3 = detail::uniform_matrix<3>;
using uniform_mat4 = detail::uniform_matrix<4>;
using uniform_mat2x3 = detail::uniform_matrix<2, 3>;
using uniform_mat3x2 = detail::uniform_matrix<3, 2>;
using uniform_mat2x4 = detail::uniform_matrix<2, 4>;
using uniform_mat4x2 = detail::uniform_matrix<4, 2>;
using uniform_mat3x4 = detail::uniform_matrix<3, 4>;
using uniform_mat4x3 = detail::uniform_matrix<4, 3>;


class TextureUnit;

class uniform_sampler : public uniform<int> {
  public:
    uniform_sampler(ProgramConstRef program, const char* name);
    uniform_sampler(ProgramConstRef program, std::string const& name);
    uniform_sampler(ProgramConstRef program, GLint location);
    ~uniform_sampler();

  public:
    void use(TextureUnit const&);

};


} // namespace gl

#endif
