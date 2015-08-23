#ifndef OGL_UNIFORM_H
#define OGL_UNIFORM_H

#include "gl_type.h"
#include <vector>

namespace gl {

/**
 * @brief handle to a uniform location
 **/
template<typename... T>
class uniform {
  public:
    uniform();
    uniform(GLint location);
    uniform(uniform<T...> const&);
    uniform<T...>& operator=(uniform<T...> const&);

  public:
    ~uniform();

  public:
    bool operator==(uniform<T...> const&) const;
    bool operator!=(uniform<T...> const&) const;

  public:
    void set(T... values);

  public:
    GLint location() const;

  private:
    GLint _location { 0 };

};

template<typename T> 
using uniform1 = uniform<T>;

template<typename T>
using uniform2 = uniform<T, T>;

template<typename T>
using uniform3 = uniform<T, T, T>;

template<typename T>
using uniform4 = uniform<T, T, T, T>;

} // namespace gl

#endif
