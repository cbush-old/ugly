

#include "uniform.h"
#include "gl_type.h"
#include "program.h"

#include <functional>

#define glUniformNTv(N, T) glUniformNT(N, T)v
#define glUniformMatrixN(NxN) glUniformMatrix##NxN##fv

namespace gl {

namespace detail {

basic_uniform::basic_uniform(Program const& program, GLint location): _program(program), _location(location) {}

GLint basic_uniform::location() const {
  return _location;
}

template<typename T>
uniform<T>::uniform(Program const& program, GLint location): basic_uniform(program, location) {}



// What is this madness? Well, it's a rather C++y way to provide a bit of type safety
// and translate to the glUniform1f, -2i, -3ui, etc., function names in a compact cpp file.
// Variadic templates are explicitly instantiated only for the types accepted by OpenGL
// while template specializations, aided by macros, do the translation to the appropriate
// OpenGL function name.
#pragma wankfest


template<typename... T>
uniform<T...>::uniform(Program const& program, GLint location)
  : basic_uniform(program, location)
  {}


#define SPECIALIZE_STEP1(...) template<> void uniform<__VA_ARGS__>
#define SPECIALIZE_STEP2(...) ::set(__VA_ARGS__)
#define SPECIALIZE_STEP3(Count, Suffix, ...) { GL_CALL(glUniform##Count##Suffix(_location, __VA_ARGS__ )); }

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


}



#define SPECIALIZE(Type, Suffix) \
  template<> void uniform2<Type>::set(vec2<Type> const& v) { GL_CALL(glUniform2##Suffix(_location, v.x, v.y)); } \
  template<> void uniform3<Type>::set(vec3<Type> const& v) { GL_CALL(glUniform3##Suffix(_location, v.x, v.y, v.z)); } \
  template<> void uniform4<Type>::set(vec4<Type> const& v) { GL_CALL(glUniform4##Suffix(_location, v.x, v.y, v.z, v.w)); } \
  template<> void uniform2<Type>::set(Type v0, Type v1) { GL_CALL(glUniform2##Suffix(_location, v0, v1)); } \
  template<> void uniform3<Type>::set(Type v0, Type v1, Type v2) { GL_CALL(glUniform3##Suffix(_location, v0, v1, v2)); } \
  template<> void uniform4<Type>::set(Type v0, Type v1, Type v2, Type v3) { GL_CALL(glUniform4##Suffix(_location, v0, v1, v2, v3)); } \
  template<> vec2<Type> uniform2<Type>::get() const { Type params[2]; GL_CALL(glGetUniform##Suffix##v(_program.name(), _location, params)); return vec2<Type>(params[0], params[1]); } \
  template<> vec3<Type> uniform3<Type>::get() const { Type params[3]; GL_CALL(glGetUniform##Suffix##v(_program.name(), _location, params)); return vec3<Type>(params[0], params[1], params[2]); } \
  template<> vec4<Type> uniform4<Type>::get() const { Type params[4]; GL_CALL(glGetUniform##Suffix##v(_program.name(), _location, params)); return vec4<Type>(params[0], params[1], params[2], params[3]); } \



SPECIALIZE(GLfloat, f);
SPECIALIZE(GLint, i);
SPECIALIZE(GLuint, ui);


}

