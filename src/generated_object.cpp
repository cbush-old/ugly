#include "gl_type.h"

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
  glDeleteFunc(1, &_name);
}

}

