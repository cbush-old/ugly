#include "shader.h"
#include "log.h"

#include <iostream>
#include <fstream>
#include <cstring>


namespace gl {

// Not much point to using pImpl while the only data member is _name.
// For now, treat _impl as reserved, but use it as _name.

class Shader_impl;

#define _name ((GLuint&)*_impl)


Shader::Shader(): _impl((Shader_impl*)new GLuint(0)) {}


Shader::~Shader() {
  GL_CALL(glDeleteShader(_name));
  delete (GLuint*)_impl;
}



std::string load_file(std::string const& path) {
  std::ifstream f(path);
  if (!f.good()) {
    throw 1;
  }
  return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}



template<GLenum Type>
Shader_type<Type>::Shader_type() {
  GL_CALL(_name = glCreateShader(Type));
}

template<GLenum Type>
Shader_type<Type>::Shader_type(std::string const& path)
  : Shader_type<Type>()
{
  std::string file = load_file(path);
  set_source(file);
  compile();
}


template<void(*f0)(GLuint, GLenum, GLint*), void(*f1)(GLuint, GLsizei, GLsizei*, GLchar*)>
void gl_log(GLuint name) {
  int log_length, max_length;
  GL_CALL(f0(name, GL_INFO_LOG_LENGTH, &max_length));
  char* log = new char[max_length];
  GL_CALL(f1(name, max_length, &log_length, log));
  if(log_length > 0) {
    log[log_length - 1] = '\0';
    logw("%s", log);
  }
  delete[] log;
}

template void gl_log<glGetProgramiv, glGetProgramInfoLog>(GLuint id);
template void gl_log<glGetShaderiv, glGetShaderInfoLog>(GLuint id);

inline static void print_log(GLuint id) {
  gl_log<glGetShaderiv, glGetShaderInfoLog>(id);
}


void Shader::compile() {

  GL_CALL(glCompileShader(_name));

  if (!compiled()) {
    print_log(_name);
    throw gl::exception("shader compilation failed");
  }

}

void Shader::set_source(std::string const& source) {
  char const * sources[1] {
    source.c_str()
  };
  GL_CALL(glShaderSource(_name, 1, sources, NULL));
}

void Shader::set_source(std::vector<std::string> const& sources) {
  char const** source = new char const*[sources.size()];
  size_t i = 0;
  for (auto const& src : sources) {
    source[i++] = src.c_str();
  }

  try {
    GL_CALL(glShaderSource(_name, (GLsizei)sources.size(), source, NULL));
  } catch(...) {
    delete[] source;
    throw;
  }
  delete[] source;
}

std::string Shader::get_source() const {
  GLsizei length = source_length();
  if (!length) {
    return "";
  }

  char* buf = new char[length];
  try {
    GL_CALL(glGetShaderSource(_name, length, &length, buf));
  } catch(...) {
    delete[] buf;
    throw;
  }
  std::string source (buf);
  delete[] buf;
  return source;
}

GLuint Shader::name() const {
  return _name;
}

template<GLenum PARAM>
inline GLint get(GLuint name) {
  GLint info;
  GL_CALL(glGetShaderiv(name, PARAM, &info));
  return info;
}

GLenum Shader::type() const {
  return (GLenum)get<GL_SHADER_TYPE>(_name);
}

bool Shader::deleted() const {
  return get<GL_DELETE_STATUS>(_name);
}

bool Shader::compiled() const {
  return get<GL_COMPILE_STATUS>(_name);
}

unsigned Shader::source_length() const {
  return (unsigned)get<GL_SHADER_SOURCE_LENGTH>(_name);
}


template class Shader_type<GL_FRAGMENT_SHADER>;
template class Shader_type<GL_VERTEX_SHADER>;
template class Shader_type<GL_TESS_CONTROL_SHADER>;
template class Shader_type<GL_TESS_EVALUATION_SHADER>;
template class Shader_type<GL_GEOMETRY_SHADER>;

} // namespace gl



