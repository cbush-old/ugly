#include "generated_object.h"

namespace gl {

typedef void(*glGenFunc)(GLsizei, GLuint*);
typedef void(*glDeleteFunc)(GLsizei, GLuint const*);

template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::GeneratedObject()
  : _owner(true) {
  GL_CALL(GenFunc(1, &_name));
}

template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::GeneratedObject(GLuint name)
  : _name(name)
  , _owner(false)
  {}


template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::~GeneratedObject() {
  if (_owner) {
    GL_CALL_NOTHROW(DeleteFunc(1, &_name));
  }
}

#define INSTANTIATE(Type) template class GeneratedObject< glGen##Type , glDelete##Type >;

INSTANTIATE(Buffers);
INSTANTIATE(Framebuffers);
INSTANTIATE(ProgramPipelines);
INSTANTIATE(Queries);
INSTANTIATE(Renderbuffers);
INSTANTIATE(Samplers);
INSTANTIATE(Textures);
INSTANTIATE(TransformFeedbacks);
INSTANTIATE(VertexArrays);

#undef INSTANTIATE

}

