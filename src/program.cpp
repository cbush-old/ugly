#include "program.h"
#include "log.h"
#include "gl_type.h"

namespace ogl {

template<void(*f0)(GLuint, GLenum, GLint*), void(*f1)(GLuint, GLsizei, GLsizei*, GLchar*)>
void gl_log(GLuint id);

template<>
void gl_log<glGetProgramiv, glGetProgramInfoLog>(GLuint id);

static inline void print_log(GLuint id) {
  gl_log<glGetProgramiv, glGetProgramInfoLog>(id);
}

Program::Program() {
  GL_CALL(_id = glCreateProgram());
  logi("Created program %d", _id);
}

Program::~Program() {
  GL_CALL(glDeleteProgram(_id));
  logi("Destroyed program %d", _id);
}

void Program::bind() const {

  glUseProgram(_id);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    loge("Error binding shader program: %d", error);
    print_log(_id);
    throw;
  }
}

void Program::unbind() const {
  GL_CALL(glUseProgram(0));
}

void Program::attach(Shader const& shader) const {
  logi("Attaching shader %d to program %d", shader.get_id(), _id);
  GL_CALL(glAttachShader(_id, shader.get_id()));
}

void Program::detach(Shader const& shader) const {
  logi("Detaching shader %d from program %d", shader.get_id(), _id);
  GL_CALL(glDetachShader(_id, shader.get_id()));
}



void Program::link() const {
  logi("Linking program %d", _id);
  GL_CALL(glLinkProgram(_id));

  int success = GL_FALSE;
  glGetProgramiv(_id, GL_LINK_STATUS, &success);

  if (success != GL_TRUE) {
    loge("Error in linking program %d", _id);
    print_log(_id);
    throw;
  }
}

void Program::enable(attribute const attr) const {

  glEnableVertexAttribArray(attr);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR){
    loge("Error enabling attribute %d for SP%d: %d", attr, _id, error);
    print_log(_id);
    throw;
  }

}

void Program::disable(attribute const attr) const {
  GL_CALL(glDisableVertexAttribArray(attr));
}

uniform Program::get_uniform(const char* name) const {
  GL_CALL(auto rv = glGetUniformLocation(_id, name));
  logv("Get uniform %s = %d", name, rv);
  return rv;
}

attribute Program::get_attrib(const char* name) const {
  GL_CALL(auto rv = glGetAttribLocation(_id, name));
  return rv;
}

void Program::set_uniform(uniform var, float const arg1) const {
  GL_CALL(glUniform1f((GLint)var, arg1));
}

void Program::set_uniform(uniform var, float const arg1, float const arg2) const {
  GL_CALL(glUniform2f((GLint)var, arg1, arg2));
}

void Program::set_uniform(uniform var, float const arg1, float const arg2, float const arg3) const {
  GL_CALL(glUniform3f((GLint)var, arg1, arg2, arg3));
}

void Program::set_uniform(uniform var, float const arg1, float const arg2, float const arg3, float const arg4) const {
  GL_CALL(glUniform4f((GLint)var,arg1,arg2,arg3,arg4));
}

void Program::set_matrix(uniform location, GLsizei const count, GLboolean const transpose, const GLfloat* value) const {
  GL_CALL(glUniformMatrix4fv((GLint)location, count, transpose, value));
}

}

