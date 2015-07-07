#include "shader.h"
#include "log.h"

#include <iostream>
#include <fstream>
#include <cstring>


namespace ogl {

template<void(*f0)(GLuint, GLenum, GLint*), void(*f1)(GLuint, GLsizei, GLsizei*, GLchar*)>
void gl_log(GLuint id) {
  int log_length, max_length;
  GL_CALL(f0(id, GL_INFO_LOG_LENGTH, &max_length));
  char* log = new char[max_length];
  GL_CALL(f1(id, max_length, &log_length, log));
  if(log_length > 0) {
    log[log_length - 1] = '\0';
    logw("%s", log);
  }
  delete[] log;
}

template<>
void gl_log<glGetProgramiv, glGetProgramInfoLog>(GLuint id);


inline static void print_log(GLuint id) {
  gl_log<glGetShaderiv, glGetShaderInfoLog>(id);
}


std::string load_file(std::string const& path) {
  std::ifstream f(path);
  if (!f.good()) {
    return "";
  }
  return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}


void Shader::load(const char* path) {

  std::string file = load_file(path);

  char const * source[1] {
    file.c_str()
  };

  if (!source[0]) {
    loge("Couldn't load shader '%s'", path);
    return;
  }

  GL_CALL(glShaderSource(_id, 1, source, NULL));

}

void Shader::compile() const {

  GL_CALL(glCompileShader(_id));

  GLint compiled = GL_FALSE;
  GL_CALL(glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled));

  if (compiled != GL_TRUE) {
    loge("Couldn't compile shader #%d!", _id); 
    print_log(_id);
    throw;
  }

}

}
