#include "buffer.h"
#include "texture.h"

namespace gl {


void Buffer::data(size_t size, void const* data, GLenum usage, GLenum target) {
  // 4.5: see glNamedBufferData
  BufferBindguard guard(target, *this);
  GL_CALL(glBufferData(target, size, data, usage));
}


void Buffer::subdata(size_t offset, size_t size, void const* data, GLenum target) {
  BufferBindguard guard(target, *this);
  GL_CALL(glBufferSubData(target, offset, size, data));
}


void* Buffer::map(GLenum target, GLenum access) {
  GL_ASSERT(!_mapped, "mapping already-mapped buffer %p", this);
  void* p;
  BufferBindguard guard(target, *this);
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
  BufferBindguard guard(_target, *this);
  GL_CALL(rv = glUnmapBuffer(_target));
  if (rv) {
    _mapped = false;
  }
  return rv;
}


void Buffer::get(size_t offset, size_t size, void* data) const {
  BufferBindguard guard(GL_COPY_READ_BUFFER, *this);
  glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, data);
}


void Buffer::texture(Texture& texture, GLenum internal_format) {
  TextureBindguard guard(GL_TEXTURE_BUFFER, texture);
  GL_CALL(glTexBuffer(GL_TEXTURE_BUFFER, internal_format, name()));
}



Buffer::Buffer() {}

Buffer::~Buffer() {}



} // namespace gl
