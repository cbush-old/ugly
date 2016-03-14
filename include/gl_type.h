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


template<typename... T>
struct vec;

template<typename U>
struct vec<U, U> {
  U x, y;
  vec() {}
  vec(U x, U y): x(x), y(y) {}
};

template<typename U>
struct vec<U, U, U> {
  U x, y, z;
  vec() {}
  vec(U x, U y, U z): x(x), y(y), z(z) {}
};

template<typename U>
struct vec<U, U, U, U> {
  U x, y, z, w;
  vec() {}
  vec(U x, U y, U z, U w): x(x), y(y), z(z), w(w) {}
};


template<typename T> using vec2 = vec<T, T>;
template<typename T> using vec3 = vec<T, T, T>;
template<typename T> using vec4 = vec<T, T, T, T>;



template<typename T>
struct rect {
  T x, y, width, height;
  rect(): x(0), y(0), width(0), height(0) {}
  rect(T x, T y, T width, T height): x(x), y(y), width(width), height(height) {}
};

using Viewport = rect<float>;


template<typename T>
bool operator==(vec2<T> const& a, vec2<T> const& b) {
  return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator==(vec3<T> const& a, vec3<T> const& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

template<typename T>
bool operator==(vec4<T> const& a, vec4<T> const& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


template<typename T>
std::ostream& operator<<(std::ostream& o, gl::vec4<T> v) {
  o << v.x << ", "
    << v.y << ", "
    << v.z << ", "
    << v.w;
  return o;
}


struct color {
  float r, g, b, a;
  color() {}
  color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}
  bool operator==(color const& o) const {
    return r == o.r && g == o.g && b == o.b && a == o.a;
  }
};

struct range {
  float low, high;
};


using attribute = GLint;


#define GL_CALL(...) \
    __VA_ARGS__; {\
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
      loge(#__VA_ARGS__ " failed: %d", error); \
      throw gl::exception("gl call " #__VA_ARGS__ " failed: %d", error); \
    } \
  }

#define GL_CALL_NOTHROW(...) \
    __VA_ARGS__; {\
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
      loge(#__VA_ARGS__ " failed: %d", error); \
    } \
  }

#define GL_VALIDATE(Type, name) {\
  GL_CALL(bool valid = glIs##Type(name)) \
  if (!valid) { throw gl::exception("%u is not a " #Type "!", name); } \
  else { logv("named object %u is a " #Type, name); } \
  }


#define GL_ASSERT(X, ...) \
  if (!(X)) { \
    throw gl::exception(__VA_ARGS__); \
  }


#define GL_BOUNDS_CHECK(I, LIMIT) GL_ASSERT(0 <= I && I < LIMIT, "index out of bounds!")


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



template<typename T, void(*BindFunction)(GLenum, GLuint)>
class Bindguard {
  public:
    Bindguard(GLenum target, T const& object);
    ~Bindguard();

  private:
    GLenum _target;

};


template<typename T, void(*BindFunction)(GLuint), GLuint Default = 0>
class NoTargetBindguard {
  public:
    NoTargetBindguard(T const& object);
    ~NoTargetBindguard();
};

class Buffer;
class Texture;
class Framebuffer;
class Renderbuffer;
class TextureUnit;
class VertexArray;
class Program;

using BufferBindguard = Bindguard<Buffer, glBindBuffer>;
using TextureBindguard = Bindguard<Texture, glBindTexture>;
using FramebufferBindguard = Bindguard<Framebuffer, glBindFramebuffer>;
using RenderbufferBindguard = Bindguard<Renderbuffer, glBindRenderbuffer>;

using ActiveTextureBindguard = NoTargetBindguard<TextureUnit, glActiveTexture, GL_TEXTURE0>;
using VertexArrayBindguard = NoTargetBindguard<VertexArray, glBindVertexArray>;
using ProgramBindguard = NoTargetBindguard<Program, glUseProgram>;


} // namespace gl

#endif
