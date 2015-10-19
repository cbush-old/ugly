#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include <vector>
#include <string>

namespace gl {

class Program;


namespace detail {

class basic_uniform {
  protected:
    basic_uniform(Program const& _program, GLint location);

  public:
    GLint location() const;

  protected:
    Program const& _program;
    GLint _location { -1 };
};

template<typename... T>
class uniform : public basic_uniform {
  using vec_t = vec<T...>;

  public:
    uniform(Program const& program, GLint location);

  public:
    void set(T... values);
    void set(vec_t const& vec);
    vec_t get() const;

};


template<typename T>
class uniform<T> : public basic_uniform {
  public:
    uniform(Program const& program, GLint location);

  public:
    void set(T value);
    T get() const;

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




} // namespace gl

#endif
