#ifndef GL_TYPE_H
#define GL_TYPE_H

#include "log.h"

#include <gl3.h>
#include <gl3ext.h>

namespace ogl {

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

template<void(*glGenFunc)(GLsizei, GLuint*), void(*glDeleteFunc)(GLsizei, const GLuint *)>
class GeneratedObject {
  public:
    GeneratedObject();
    virtual ~GeneratedObject() =0;

  private:
    GLuint _id;
};

#define GENERATED_OBJECT(names) GeneratedObject< glGen##names , glDelete##names >

class Buffer : public GENERATED_OBJECT(Buffers) {
  public:
    virtual ~Buffer() {}
};

class Framebuffer : public GENERATED_OBJECT(Framebuffers) {
  public:
    virtual ~Framebuffer() {}

};

class Pipeline : public GENERATED_OBJECT(ProgramPipelines) {
  public:
    virtual ~Pipeline() {}
};

class Query : public GENERATED_OBJECT(Queries) {
  public:
    virtual ~Query() {}
};

class Renderbuffer : public GENERATED_OBJECT(Renderbuffers) {
  public:
    virtual ~Renderbuffer() {}
};

class Sampler : public GENERATED_OBJECT(Samplers) {
  public:
    virtual ~Sampler() {}
};

class Texture : public GENERATED_OBJECT(Textures) {
  public:
    virtual ~Texture() {}
};

class TransformFeedback : public GENERATED_OBJECT(TransformFeedbacks) {
  public:
    virtual ~TransformFeedback() {}

};

class VertexArray : public GENERATED_OBJECT(VertexArrays) {
  public:
    virtual ~VertexArray() {}

};

} // namespace ogl

#endif
