#include "buffer.h"

namespace gl {

class Bindguard {
  public:
    Bindguard(GLenum target, GLuint name)
      : _target(target)
      , _name(name) {
      GL_CALL(glBindBuffer(_target, _name));
    }

    ~Bindguard() {
      GL_CALL(glBindBuffer(_target, 0));
    }

  private:
    GLenum _target;
    GLuint _name;

};



void Buffer::data(size_t size, void const* data, GLenum usage, GLenum target) {
  // 4.5: see glNamedBufferData
  Bindguard guard(target, name());
  GL_CALL(glBufferData(target, size, data, usage));
}


void Buffer::subdata(size_t offset, size_t size, void const* data) {
  Bindguard guard(GL_COPY_WRITE_BUFFER, name());
  GL_CALL(glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data));
}


void* Buffer::map(GLenum target, GLenum access) {
  GL_ASSERT(!_mapped, "mapping already-mapped buffer %p", this);
  void* p;
  Bindguard guard(target, name());
  GL_CALL(p = glMapBuffer(target, access));
  if (p) {
    _target = target;
    _mapped = true;
  }
  return p;
}

bool Buffer::unmap() {
  GL_ASSERT(_mapped, "unmapping buffer %p, which is not mapped", this);
  bool rv;
  Bindguard guard(_target, name());
  GL_CALL(rv = glUnmapBuffer(_target));
  if (rv) {
    _mapped = false;
  }
  return rv;
}


void Buffer::get(size_t offset, size_t size, void* data) const {
  Bindguard guard(GL_COPY_READ_BUFFER, name());
  glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, data);
}



Buffer::Buffer() {}

Buffer::~Buffer() {}



} // namespace gl
