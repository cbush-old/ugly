

#include "uniform.h"
#include "gl_type.h"
#include "program.h"
#include "texture_unit.h"

#include <functional>

namespace gl {

namespace detail {

basic_uniform::basic_uniform(untyped_uniform u)
  : _program(u.program())
  , _location(u.location())
  {}

basic_uniform::~basic_uniform() {}

GLint basic_uniform::location() const {
  return _location;
}

/*
template<typename T>
uniform<T>::uniform(untyped_uniform u)
  : basic_uniform(u)
  {}
*/


// What is this madness? Well, it's a rather C++y way to provide a bit of type safety
// and translate to the glUniform1f, -2i, -3ui, etc., function names in a compact cpp file.
// Variadic templates are explicitly instantiated only for the types accepted by OpenGL
// while template specializations, aided by macros, do the translation to the appropriate
// OpenGL function name.
#pragma wankfest


template<typename... T>
uniform<T...>::uniform(untyped_uniform u)
  : basic_uniform(u)
  {}

template<typename... T>
uniform<T...>::~uniform() {}


// uniform scalar specialization
#define SPECIALIZE_STEP1(...) template<> void uniform<__VA_ARGS__>
#define SPECIALIZE_STEP2(...) ::set(__VA_ARGS__)
#define SPECIALIZE_STEP3(Count, Suffix, ...) { \
  ProgramBindguard guard(_program->name()); \
  GL_CALL(glUniform##Count##Suffix(_location, __VA_ARGS__ )); \
}

#define SPECIALIZE2(Type, Suffix) \
  SPECIALIZE_STEP1(Type, Type) \
  SPECIALIZE_STEP2(Type v0, Type v1) \
  SPECIALIZE_STEP3(2, Suffix, v0, v1)

#define SPECIALIZE3(Type, Suffix) \
  SPECIALIZE_STEP1(Type, Type, Type) \
  SPECIALIZE_STEP2(Type v0, Type v1, Type v2) \
  SPECIALIZE_STEP3(3, Suffix, v0, v1, v2)

#define SPECIALIZE4(Type, Suffix) \
  SPECIALIZE_STEP1(Type, Type, Type, Type) \
  SPECIALIZE_STEP2(Type v0, Type v1, Type v2, Type v3) \
  SPECIALIZE_STEP3(4, Suffix, v0, v1, v2, v3)

#define SPECIALIZE_TYPE(Type, Suffix) \
  SPECIALIZE2(Type, Suffix) \
  SPECIALIZE3(Type, Suffix) \
  SPECIALIZE4(Type, Suffix)

#define INSTANTIATE(...) template class uniform<__VA_ARGS__>;
#define INSTANTIATE_TYPE(T) \
  INSTANTIATE(T); \
  INSTANTIATE(T, T); \
  INSTANTIATE(T, T, T); \
  INSTANTIATE(T, T, T, T);

INSTANTIATE_TYPE(GLfloat);
INSTANTIATE_TYPE(GLint);
INSTANTIATE_TYPE(GLuint);
#undef INSTANTIATE
#undef INSTANTIATE_TYPE
#undef SPECIALIZE_STEP1
#undef SPECIALIZE_STEP2
#undef SPECIALIZE_STEP3
#undef SPECIALIZE2
#undef SPECIALIZE3
#undef SPECIALIZE4




template<unsigned N, unsigned M>
uniform_matrix<N, M>::uniform_matrix(untyped_uniform u)
  : basic_uniform(u)
  , _count(1) // TODO
  {}

template<unsigned N, unsigned M>
uniform_matrix<N, M>::~uniform_matrix() {}


#define SPECIALIZE_AND_INSTANTIATE(N, M, SUFFIX) \
  template<> \
  void uniform_matrix<N, M>::set(GLfloat const* value, bool transpose) { \
    ProgramBindguard guard(_program->name()); \
    GL_CALL(glUniformMatrix##SUFFIX##fv(_location, _count, (GLboolean)transpose, value)); \
  } \
  template class uniform_matrix< N, M >;


SPECIALIZE_AND_INSTANTIATE(2, 2, 2);
SPECIALIZE_AND_INSTANTIATE(3, 3, 3);
SPECIALIZE_AND_INSTANTIATE(4, 4, 4);
SPECIALIZE_AND_INSTANTIATE(2, 3, 2x3);
SPECIALIZE_AND_INSTANTIATE(3, 2, 3x2);
SPECIALIZE_AND_INSTANTIATE(2, 4, 2x4);
SPECIALIZE_AND_INSTANTIATE(4, 2, 4x2);



#undef SPECIALIZE_AND_INSTANTIATE





} // namespace detail





#define SPECIALIZE(Type, Suffix) \
  template<> void uniform2<Type>::set(vec2<Type> const& v) { GL_CALL(glProgramUniform2##Suffix(_program->name(), _location, v.x, v.y)); } \
  template<> void uniform3<Type>::set(vec3<Type> const& v) { GL_CALL(glProgramUniform3##Suffix(_program->name(), _location, v.x, v.y, v.z)); } \
  template<> void uniform4<Type>::set(vec4<Type> const& v) { GL_CALL(glProgramUniform4##Suffix(_program->name(), _location, v.x, v.y, v.z, v.w)); } \
  template<> void uniform<Type>::set(Type v0) { GL_CALL(glProgramUniform1##Suffix(_program->name(), _location, v0)); } \
  template<> void uniform2<Type>::set(Type v0, Type v1) { GL_CALL(glProgramUniform2##Suffix(_program->name(), _location, v0, v1)); } \
  template<> void uniform3<Type>::set(Type v0, Type v1, Type v2) { GL_CALL(glProgramUniform3##Suffix(_program->name(), _location, v0, v1, v2)); } \
  template<> void uniform4<Type>::set(Type v0, Type v1, Type v2, Type v3) { GL_CALL(glProgramUniform4##Suffix(_program->name(), _location, v0, v1, v2, v3)); } \
  template<> vec2<Type> uniform2<Type>::get() const { Type params[2]; GL_CALL(glGetUniform##Suffix##v(_program->name(), _location, params)); return vec2<Type>(params[0], params[1]); } \
  template<> vec3<Type> uniform3<Type>::get() const { Type params[3]; GL_CALL(glGetUniform##Suffix##v(_program->name(), _location, params)); return vec3<Type>(params[0], params[1], params[2]); } \
  template<> vec4<Type> uniform4<Type>::get() const { Type params[4]; GL_CALL(glGetUniform##Suffix##v(_program->name(), _location, params)); return vec4<Type>(params[0], params[1], params[2], params[3]); } \


SPECIALIZE(GLfloat, f);
SPECIALIZE(GLint, i);
SPECIALIZE(GLuint, ui);






uniform_sampler::uniform_sampler(untyped_uniform u)
  : uniform<int>(u) {}

void uniform_sampler::use(TextureUnit const& unit) {
  uniform<int>::set(unit.unit());
}

uniform_sampler::~uniform_sampler() {}



untyped_uniform::untyped_uniform(Program const* program, GLint location)
  : _program(program)
  , _location(location)
  {}

untyped_uniform::untyped_uniform(untyped_uniform const& o)
  : _program(o._program)
  , _location(o._location)
{}

untyped_uniform::~untyped_uniform() {}

GLint untyped_uniform::location() const {
  return _location;
}

Program const* untyped_uniform::program() const {
  return _program;
}




}

