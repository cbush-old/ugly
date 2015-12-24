#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include "program.h"

#include <vector>
#include <string>

namespace gl {


class TextureUnit;
class untyped_uniform;


class attrib {
  public:
    attrib(ProgramConstRef, GLint location);

  public:
    GLint location() const;

  protected:
    ProgramConstRef _program;
    GLint _location { -1 };

};


namespace detail {

class basic_uniform {
  protected:
    basic_uniform(untyped_uniform const&);

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
    uniform(untyped_uniform const&);

  public:
    void set(T... values);
    void set(vec_t const& vec);
    vec_t get() const;

};


template<unsigned N, unsigned M = N>
class uniform_matrix : public basic_uniform {
  public:
    uniform_matrix(untyped_uniform const&, GLsizei count = 1);

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


class uniform_sampler : public uniform<int> {
  public:
    uniform_sampler(untyped_uniform const&);

  public:
    void set(TextureUnit const&);

};

class untyped_uniform {
  public:
    untyped_uniform(ProgramConstRef program, GLint location);
  
  public:
    ProgramConstRef program() const;
    GLint location() const;
  
  public:
    template<typename... T>
    void set(T... values) {
      detail::uniform<T...>(*this).set(values...);
    }
  
    template<typename... T>
    void set(vec<T...> value) {
      detail::uniform<T...>(*this).set(value);
    }

    void set(TextureUnit const& value) {
      uniform_sampler(*this).set(value);
    }

  private:
    ProgramConstRef _program;
    GLint _location { -1 };

};



} // namespace gl

#endif
