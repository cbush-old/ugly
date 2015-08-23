#include "generated_object.h"

namespace gl {

typedef void(*glGenFunc)(GLsizei, GLuint*);
typedef void(*glDeleteFunc)(GLsizei, GLuint const*);

template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::GeneratedObject() {
  GL_CALL(GenFunc(1, &_name));
}

template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::GeneratedObject(GLuint name): _name(name) {}


template<glGenFunc GenFunc, glDeleteFunc DeleteFunc>
GeneratedObject<GenFunc, DeleteFunc>::~GeneratedObject() {
  GL_CALL(DeleteFunc(1, &_name));
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


}

