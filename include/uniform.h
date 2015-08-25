#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include <vector>
#include <string>

namespace gl {

class Program;

/**
 * @brief handle to a uniform location
 **/
template<typename... T>
class uniform {
  public:
    uniform();
    uniform(GLint location);

    /**
     * @brief retrieve the uniform by name from the given program
     **/
    uniform(Program const& program, const char* name);
    uniform(Program const& program, std::string const& name);

  public:
    uniform(uniform<T...> const&);
    uniform<T...>& operator=(uniform<T...> const&);
    ~uniform();

  public:
    bool operator==(uniform<T...> const&) const;
    bool operator!=(uniform<T...> const&) const;

  public:
    void set(T... values);

    template<typename U>
    void set(U const& vec);

  public:
    GLint location() const;

  private:
    GLint _location { -1 };

};

template<typename T> 
using uniform1 = uniform<T>;

template<typename T>
using uniform2 = uniform<T, T>;

template<typename T>
using uniform3 = uniform<T, T, T>;

template<typename T>
using uniform4 = uniform<T, T, T, T>;


#define SPECIALIZE1(...) template<> template<typename U> void uniform<__VA_ARGS__>
#define SPECIALIZE2(...) ::set(U const& vec) { set(__VA_ARGS__); }
#define SPECIALIZE(T) \
  SPECIALIZE1(T) SPECIALIZE2(vec.x) \
  SPECIALIZE1(T, T) SPECIALIZE2(vec.x, vec.y) \
  SPECIALIZE1(T, T, T) SPECIALIZE2(vec.x, vec.y, vec.z) \
  SPECIALIZE1(T, T, T, T) SPECIALIZE2(vec.x, vec.y, vec.z, vec.w)

SPECIALIZE(GLfloat);
SPECIALIZE(GLint);
SPECIALIZE(GLuint);

#undef SPECIALIZE
#undef SPECIALIZE1
#undef SPECIALIZE2


} // namespace gl

#endif
