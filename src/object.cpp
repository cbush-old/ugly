#include "gl_type.h"

template<class GenFunc, class DeleteFunc>
GeneratedObject::GeneratedObject() {
  GL_CALL(GenFunc(1, &_id));
}

template<class GenFunc, class DeleteFunc>
GeneratedObject::~GeneratedObject() {
  GL_CALL(DeleteFunc(1, &_id));
}