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


class Program {
  public:
    Program();
    ~Program();
    Program(Program const&) = delete;
    Program& operator=(Program const&) = delete;

  public:
    inline GLuint name() const { return _name; }

  public:
    template<typename StringType>
    GLint find_cached_attrib_location(StringType const& name) const;
  
    template<typename StringType>
    GLint attrib_location(StringType const& name) const;

    void clear_cache();

  private:
    mutable std::unordered_map<GLint, GLint> _attrib_cache;
    GLuint _name;

};

namespace {

#define HASH_CHAR(x, c) x ^= (c) * 0x7fffffff;

GLint hash(const char* s) {
  GLint x = 0;
  for (const char* c = s; *c != '\0'; ++c) {
    HASH_CHAR(x, *c);
  }
  return x;
}

GLint hash(std::string const& s) {
  GLint x = 0;
  for (auto const& c : s) {
    HASH_CHAR(x, c);
  }
  return x;
}

inline const char* c_str(std::string const& s) {
  return s.c_str();
}

inline const char* c_str(const char* s) {
  return s;
}

#undef HASH_CHAR

}


template<typename StringType>
GLint Program::attrib_location(StringType const& attrib_name) const {
  GLint location = find_cached_attrib_location(attrib_name);
  if (location == -1) {
    GL_CALL(location = glGetAttribLocation(_name, c_str(attrib_name)));
    _attrib_cache[hash(attrib_name)] = location;
  }
  return location;
}


template<typename StringType>
GLint Program::find_cached_attrib_location(StringType const& name) const {
  auto it = _attrib_cache.find(hash(name));
  if (it != _attrib_cache.end()) {
    return it->second;
  }
  return -1;
}


void Program::clear_cache() {
  _attrib_cache.clear();
}


Program::Program(): _name(0) {
  GL_CALL(_name = glCreateProgram());
}

Program::~Program() {
  GL_CALL(glDeleteProgram(_name));
}

ProgramRef::ProgramRef()
  : _shared(new Program())
  {}

ProgramRef::ProgramRef(ProgramRef const& o)
  : _shared(o._shared)
  {}

ProgramRef& ProgramRef::operator=(ProgramRef const& o) {
  _shared = o._shared;
  return *this;
}

ProgramRef::~ProgramRef() {}


void ProgramRef::attach(Shader const& shader) {
  GL_VALIDATE(Shader, shader.name());
  GL_CALL(glAttachShader(name(), shader.name()));
}

void ProgramRef::detach(Shader const& shader) {
  GL_VALIDATE(Shader, shader.name());
  GL_CALL(glDetachShader(name(), shader.name()));
}


void ProgramRef::link() {
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

GLuint ProgramRef::name() const {
  return _shared->name();
}

GLint ProgramRef::uniform_location(const char* uniform_name) const {
  GL_CALL(GLint location = glGetUniformLocation(name(), uniform_name));
  return location;
}

GLint ProgramRef::uniform_location(std::string const& uniform_name) const {
  GL_CALL(GLint location = glGetUniformLocation(name(), uniform_name.c_str()));
  return location;
}

GLint ProgramRef::attrib_location(const char* attrib_name) const {
  return _shared->attrib_location(attrib_name);
}

GLint ProgramRef::attrib_location(std::string const& attrib_name) const {
  return _shared->attrib_location(attrib_name);
}

attrib ProgramRef::attrib(const char* name) const {
  return { *this, attrib_location(name) };
}

attrib ProgramRef::attrib(std::string const& name) const {
  return { *this, attrib_location(name) };
}


untyped_uniform ProgramRef::operator[](const char* name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform ProgramRef::operator[](std::string const& name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform ProgramRef::uniform(const char* name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform ProgramRef::uniform(std::string const& name) const {
  return { *this, uniform_location(name) };
}

untyped_uniform ProgramRef::uniform(GLint location) const {
  return { *this, location };
}


GLint ProgramRef::get(GLenum param) const {
  GLint params[3];
  GL_CALL(glGetProgramiv(name(), param, params));
  return params[0]; // fixme: GL_COMPUTE_WORK_GROUP_SIZE returns an array
}

Binary ProgramRef::binary() const {
  GLsizei size { get(GL_PROGRAM_BINARY_LENGTH) }, length { 0 };
  Binary binary (size);
  GL_CALL(glGetProgramBinary(name(), size, &length, &binary.format, binary.buffer.data()));
  return binary;
}

void ProgramRef::binary(Binary const& binary) {
  GL_CALL(glProgramBinary(name(), binary.format, binary.buffer.data(), (GLsizei)binary.buffer.size()));
}

GLint ProgramRef::stage(GLenum shadertype, GLenum param) const {
  GLint value;
  GL_CALL(glGetProgramStageiv(name(), shadertype, param, &value));
  return value;
}

void ProgramRef::parameter(GLenum param, GLint value) {
  GL_CALL(glProgramParameteri(name(), param, value));
}

bool ProgramRef::validate() const {
  GL_CALL(glValidateProgram(name()));
  return get(GL_VALIDATE_STATUS);
}

std::string ProgramRef::info_log() const {
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

uniform_info ProgramRef::active_uniform(GLuint index) const {
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




} // namespace gl




