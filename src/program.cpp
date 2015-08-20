#include "program.h"
#include "log.h"
#include "gl_type.h"

namespace gl {

template<void(*f0)(GLuint, GLenum, GLint*), void(*f1)(GLuint, GLsizei, GLsizei*, GLchar*)>
void gl_log(GLuint id);

template<>
void gl_log<glGetProgramiv, glGetProgramInfoLog>(GLuint id);

static inline void print_log(GLuint id) {
  gl_log<glGetProgramiv, glGetProgramInfoLog>(id);
}

Program::Program(IContext& context):
  ContextAssociatedObject(context) {
  GL_CALL(_name = glCreateProgram());
}

Program::~Program() {
  GL_CALL(glDeleteProgram(_name));
}
/*
void Program::attach(Shader const& shader) const {
  logi("Attaching shader %d to program %d", shader.get_name(), _name);
  GL_CALL(glAttachShader(_name, shader.get_name()));
}

void Program::detach(Shader const& shader) const {
  logi("Detaching shader %d from program %d", shader.get_name(), _name);
  GL_CALL(glDetachShader(_name, shader.get_name()));
}

*/

void Program::link() {
  logi("Linking program %d", _name);
  GL_CALL(glLinkProgram(_name));

  int success = GL_FALSE;
  glGetProgramiv(_name, GL_LINK_STATUS, &success);

  if (success != GL_TRUE) {
    loge("Error in linking program %d", _name);
    print_log(_name);
    throw;
  }
}


/*
void Program::enable(attribute const attr) const {

  glEnableVertexAttribArray(attr);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR){
    loge("Error enabling attribute %d for SP%d: %d", attr, _name, error);
    print_log(_name);
    throw;
  }

}

void Program::disable(attribute const attr) const {
  GL_CALL(glDisableVertexAttribArray(attr));
}

uniform Program::get_uniform(const char* name) const {
  GL_CALL(auto rv = glGetUniformLocation(_name, name));
  logv("Get uniform %s = %d", name, rv);
  return rv;
}

attribute Program::get_attrib(const char* name) const {
  GL_CALL(auto rv = glGetAttribLocation(_name, name));
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
*/

}

