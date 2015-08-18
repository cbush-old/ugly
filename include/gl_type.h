#ifndef GL_TYPE_H
#define GL_TYPE_H

#include "log.h"

#include <gl3.h>
#include <gl3ext.h>

namespace gl {

template<typename T>
struct vec2_t {
  union { T x, low, near; };
  union { T y, high, far; };
};

template<typename T>
struct vec4_t {
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };
};

using range = vec2_t<float>;
using vec4 = vec4_t<float>;
using color = vec4;

using attribute = GLint;


#define GL_CALL(x) \
    x; {\
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
      loge(#x " failed: %d", error); \
      throw; \
    } \
  }

#define DECL_GL_OBJECT(Class) \
  private: GLuint _name { 0 }; \
           Class(); \
           Class(GLuint); \
  public: ~Class();

#define DEFINE_GL_OBJECT(Class, Plural) \
  Class::Class() { GL_CALL(glGen##Plural(1, &_name)); } \
  Class::Class(GLuint name): _name(name) {} \
  Class::~Class() { try { GL_CALL(glDelete##Plural(1, &_name)); } catch(...){} }


} // namespace gl

#endif
