#include "program.h"
#include "log.h"
#include "context.h"
#include "uniform.h"

namespace gl {

template<void(*f0)(GLuint, GLenum, GLint*), void(*f1)(GLuint, GLsizei, GLsizei*, GLchar*)>
void gl_log(GLuint id);

static inline void print_log(GLuint id) {
  gl_log<glGetProgramiv, glGetProgramInfoLog>(id);
}


Program::Program()
  : _name(0)
{
  GL_CALL(_name = glCreateProgram());
}

Program::~Program() {
  GL_CALL(glDeleteProgram(_name));
}


void Program::attach(Shader const& shader) {
  GL_VALIDATE(Shader, shader.name());
  GL_CALL(glAttachShader(name(), shader.name()));
}

void Program::detach(Shader const& shader) {
  GL_VALIDATE(Shader, shader.name());
  GL_CALL(glDetachShader(name(), shader.name()));
}


void Program::link() {
  auto name = this->name();
  GL_VALIDATE(Program, name);
  GL_CALL(glLinkProgram(name));

  int success = GL_FALSE;
  glGetProgramiv(name, GL_LINK_STATUS, &success);

  if (success != GL_TRUE) {
    print_log(name);
    throw gl::exception("failed to link program %u", name);
  }
}

GLuint Program::name() const {
  return _name;
}

GLint Program::uniform_location(const char* uniform_name) const {
  GL_CALL(GLint location = glGetUniformLocation(name(), uniform_name));
  return location;
}

GLint Program::uniform_location(std::string const& uniform_name) const {
  GL_CALL(GLint location = glGetUniformLocation(name(), uniform_name.c_str()));
  return location;
}

GLint Program::attrib_location(const char* attrib_name) const {
  int location;
  GL_CALL(location = glGetAttribLocation(_name, attrib_name));
  return location;
}

GLint Program::attrib_location(std::string const& attrib_name) const {
  return attrib_location(attrib_name.c_str());
}

attrib Program::attrib(const char* name) const {
  return { attrib_location(name) };
}

attrib Program::attrib(std::string const& name) const {
  return { attrib_location(name) };
}


untyped_uniform Program::operator[](const char* name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform Program::operator[](std::string const& name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform Program::uniform(const char* name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform Program::uniform(std::string const& name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform Program::uniform(GLint location) const {
  return { *this, location };
}


GLint Program::get(GLenum param) const {
  GLint params[3];
  GL_CALL(glGetProgramiv(name(), param, params));
  return params[0]; // fixme: GL_COMPUTE_WORK_GROUP_SIZE returns an array
}

Binary Program::binary() const {
  GLsizei size { get(GL_PROGRAM_BINARY_LENGTH) }, length { 0 };
  Binary binary (size);
  GL_CALL(glGetProgramBinary(name(), size, &length, &binary.format, binary.buffer.data()));
  return binary;
}

void Program::binary(Binary const& binary) {
  GL_CALL(glProgramBinary(name(), binary.format, binary.buffer.data(), (GLsizei)binary.buffer.size()));
}

GLint Program::stage(GLenum shadertype, GLenum param) const {
  GLint value;
  GL_CALL(glGetProgramStageiv(name(), shadertype, param, &value));
  return value;
}

void Program::parameter(GLenum param, GLint value) {
  GL_CALL(glProgramParameteri(name(), param, value));
}

bool Program::validate() const {
  GL_CALL(glValidateProgram(name()));
  return get(GL_VALIDATE_STATUS);
}

std::string Program::info_log() const {
  GLsizei length { get(GL_INFO_LOG_LENGTH) };
  if (!length) {
    return "";
  }
  char *buffer = new char[length];
  try {
    GL_CALL(glGetProgramInfoLog(name(), length, nullptr, buffer));
    std::string s (buffer, length);
    delete[] buffer;
    return s;
  } catch(...) {
    delete[] buffer;
    throw;
  }
}

uniform_info Program::active_uniform(GLuint index) const {
  GLsizei bufSize = get(GL_ACTIVE_UNIFORM_MAX_LENGTH), length;
  uniform_info info;
  char* buf = new char[bufSize];
  try {
    GL_CALL(glGetActiveUniform(name(), index, bufSize, &length, &info.size, &info.type, buf));
    info.name = buf;
    delete[] buf;
    return info;
  } catch(...) {
    delete[] buf;
    throw;
  }
}


GLuint Program::uniform_block_index(const char* uniform_name) const {
  GL_CALL(return glGetUniformBlockIndex(name(), uniform_name));
}

GLuint Program::uniform_block_index(std::string const& uniform_name) const {
  return uniform_block_index(uniform_name.c_str());
}



} // namespace gl




