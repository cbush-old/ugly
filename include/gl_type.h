#ifndef GL_TYPE_H
#define GL_TYPE_H

#include "log.h"

#include <gl3.h>
#include <gl3ext.h>

#include <map>
#include <thread>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

#include "exception.h"

namespace gl {

template<typename T>
struct vec2_t {
  union { T x, low, near; };
  union { T y, high, far; };
};

template<typename T>
struct vec4_t {
  vec4_t() {}
  vec4_t(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {} 
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };

  bool operator==(vec4_t const& o) {
    return x == o.x
      && y == o.y
      && z == o.z
      && w == o.w;
  }
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
      throw gl::exception("gl call " #x " failed: %d", error); \
    } \
  }

#define GL_VALIDATE(Type, name) {\
  GL_CALL(bool valid = glIs##Type(name)) \
  if (!valid) { throw gl::exception("%u is not a " #Type "!", name); } \
  else { logv("named object %u is a " #Type, name); } \
  }



enum BufferIndex {
  BUFFER_INDEX_ARRAY = 0,
  // BUFFER_INDEX_ATOMIC_COUNTER, // 4.2+
  BUFFER_INDEX_COPY_READ,
  BUFFER_INDEX_COPY_WRITE,
  // BUFFER_INDEX_DISPATCH_INDIRECT, // 4.3+
  BUFFER_INDEX_DRAW_INDIRECT,
  BUFFER_INDEX_ELEMENT_ARRAY,
  BUFFER_INDEX_PIXEL_PACK,
  BUFFER_INDEX_PIXEL_UNPACK,
  // BUFFER_INDEX_QUERY, // 4.4+
  // BUFFER_INDEX_SHADER_STORAGE, // 4.3+
  BUFFER_INDEX_TEXTURE,
  BUFFER_INDEX_TRANSFORM_FEEDBACK,
  BUFFER_INDEX_UNIFORM,
  BUFFER_INDEX_MAX
};



} // namespace gl

#endif
