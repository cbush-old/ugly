#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include <vector>
#include <string>

namespace gl {

class Program;



class untyped_uniform {
  public:
    untyped_uniform(Program const* program, GLint location);

  public:
    untyped_uniform& operator=(untyped_uniform const&) = delete;
    untyped_uniform(untyped_uniform const&);
    ~untyped_uniform();
  
  public:
    GLint location() const;
  
  public:
    Program const* program() const;
  
  private:
    Program const* _program { nullptr };
    GLint _location;
};



namespace detail {

class basic_uniform {
  protected:
    basic_uniform();
    basic_uniform(untyped_uniform);
    ~basic_uniform();

  public:
    GLint location() const;

  protected:
    Program const* _program { nullptr };
    GLint _location { -1 };
};

template<typename... T>
class uniform : public basic_uniform {
  using vec_t = vec<T...>;
  
  public:
    uniform();
    uniform(untyped_uniform);
    uniform& operator=(untyped_uniform);
    ~uniform();

  public:
    void set(T... values);
    void set(vec_t const& vec);
    vec_t get() const;

};

/*
template<typename T>
class uniform<T> : public basic_uniform {
  public:
    void set(T value);
    T get() const;

};
*/
  
  
template<unsigned N, unsigned M = N>
class uniform_matrix : public basic_uniform {
  public:
    uniform_matrix();
    uniform_matrix(untyped_uniform);
    uniform_matrix& operator=(untyped_uniform);
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
    uniform_sampler();
    uniform_sampler(untyped_uniform);
    uniform_sampler& operator=(untyped_uniform);
    ~uniform_sampler();

  public:
    void use(TextureUnit const&);

};


} // namespace gl

#endif
