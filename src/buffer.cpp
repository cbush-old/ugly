#include "buffer.h"

namespace gl {


void Buffer::data(size_t size, void const* data, GLenum usage, GLenum target) {
  // 4.5: see glNamedBufferData
  BufferBindguard guard(target, name());
  GL_CALL(glBufferData(target, size, data, usage));
}


void Buffer::subdata(size_t offset, size_t size, void const* data) {
  BufferBindguard guard(GL_COPY_WRITE_BUFFER, name());
  GL_CALL(glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, data));
}


void* Buffer::map(GLenum target, GLenum access) {
  GL_ASSERT(!_mapped, "mapping already-mapped buffer %p", this);
  void* p;
  BufferBindguard guard(target, name());
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
  BufferBindguard guard(_target, name());
  GL_CALL(rv = glUnmapBuffer(_target));
  if (rv) {
    _mapped = false;
  }
  return rv;
}


void Buffer::get(size_t offset, size_t size, void* data) const {
  BufferBindguard guard(GL_COPY_READ_BUFFER, name());
  glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, data);
}



Buffer::Buffer() {}

Buffer::~Buffer() {}



} // namespace gl
