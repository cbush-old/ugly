

#include "uniform.h"
#include "gl_type.h"
#include "program.h"

#include <functional>

#define glUniformNTv(N, T) glUniformNT(N, T)v
#define glUniformMatrixN(NxN) glUniformMatrix##NxN##fv

namespace gl {


// What is this madness? Well, it's a very C++y way to provide a bit of type safety
// and translate to the glUniform1f, -2i, -3ui, etc., function names in a compact cpp file.
// Variadic templates are explicitly instantiated only for the types accepted by OpenGL
// while template specializations, aided by macros, do the translation to the appropriate
// OpenGL function name.
#pragma wankfest


template<typename... T>
uniform<T...>::uniform() {}


template<typename... T>
uniform<T...>::uniform(GLint location)
  : _location(location)
  {}


template<typename... T>
uniform<T...>::uniform(Program const& program, const char* name)
  : _location(program.uniform_location(name))
  {}


template<typename... T>
uniform<T...>::uniform(Program const& program, std::string const& name)
  : _location(program.uniform_location(name))
  {}


template<typename... T>
uniform<T...>::uniform(uniform<T...> const& other)
  : _location(other._location)
  {}


template<typename... T>
uniform<T...>& uniform<T...>::operator=(uniform<T...> const& other) {
  _location = other._location;
  return *this;
}


template<typename... T>
uniform<T...>::~uniform() {}

template<typename... T>
bool uniform<T...>::operator==(uniform<T...> const& other) const {
  return _location == other._location;
}

template<typename... T>
bool uniform<T...>::operator!=(uniform<T...> const& other) const {
  return !(*this == other);
}

template<typename... T>
GLint uniform<T...>::location() const {
  return _location;
}




#define SPECIALIZE_STEP1(...) template<> void uniform<__VA_ARGS__>
#define SPECIALIZE_STEP2(...) ::set(__VA_ARGS__)
#define SPECIALIZE_STEP3(Count, Suffix, ...) { GL_CALL(glUniform##Count##Suffix(_location, __VA_ARGS__ )); }

#define SPECIALIZE1(Type, Suffix) \
  SPECIALIZE_STEP1(Type) \
  SPECIALIZE_STEP2(Type v0) \
  SPECIALIZE_STEP3(1, Suffix, v0)

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
  SPECIALIZE1(Type, Suffix) \
  SPECIALIZE2(Type, Suffix) \
  SPECIALIZE3(Type, Suffix) \
  SPECIALIZE4(Type, Suffix)

#define INSTANTIATE(...) template class uniform<__VA_ARGS__>;
#define INSTANTIATE_TYPE(T) \
  INSTANTIATE(T); \
  INSTANTIATE(T, T); \
  INSTANTIATE(T, T, T); \
  INSTANTIATE(T, T, T, T);

#define SPECIALIZE_AND_INSTANTIATE(Type, Suffix) \
  SPECIALIZE_TYPE(Type, Suffix) \
  INSTANTIATE_TYPE(Type);

SPECIALIZE_AND_INSTANTIATE(GLfloat, f);
SPECIALIZE_AND_INSTANTIATE(GLint, i);
SPECIALIZE_AND_INSTANTIATE(GLuint, ui);


}

