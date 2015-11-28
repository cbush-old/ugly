#include "texture.h"
#include "buffer.h"

namespace gl {




Texture::Texture(GLenum target, GLenum internal_format)
  : _target(target)
  , _internal_format(internal_format)
  {}

Texture::Texture(GLuint name, GLenum target, GLenum internal_format)
  : GeneratedObject(name)
  , _target(target)
  , _internal_format(internal_format)
  {}

Texture::~Texture() {}



Texture2D::Texture2D(GLenum internal_format /* = GL_RGBA */)
  : Texture(GL_TEXTURE_2D, internal_format)
  {}


void Texture2D::image(int level, ImageDesc2D const& desc) {
  TextureBindguard guard(_target, name());
  GL_CALL(glTexImage2D(
    _target,
    level,
    _internal_format,
    desc.width,
    desc.height,
    0, // border: always 0
    desc.format,
    desc.type,
    desc.data
  ));
}

void Texture2D::subimage(int level, unsigned xoffset, unsigned yoffset, ImageDesc2D const& desc) {
  TextureBindguard guard(_target, name());
  GL_CALL(glTexSubImage2D(
    _target,
    level,
    xoffset,
    yoffset,
    desc.width,
    desc.height,
    desc.format,
    desc.type,
    desc.data
  ));
}

void Texture2D::unpack(int level, Buffer const& buffer, ImageDesc2D const& desc, size_t offset) {
  BufferBindguard buffer_guard(GL_PIXEL_UNPACK_BUFFER, buffer.name());
  TextureBindguard texture_guard(_target, name());
  GL_CALL(glTexImage2D(
    _target,
    level,
    _internal_format,
    desc.width,
    desc.height,
    0, // border: always 0
    desc.format,
    desc.type,
    (void const*)offset
  ));
}

void Texture2D::copy(int level, Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  BufferBindguard buffer_guard(GL_READ_BUFFER, buffer.name());
  TextureBindguard texture_guard(_target, name());
  GL_CALL(glCopyTexImage2D(
    _target,
    level,
    _internal_format,
    x,
    y,
    w,
    h,
    0 // border: always 0
  ));
}

void Texture2D::subcopy(int level, unsigned xoffset, unsigned yoffset,
  Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  BufferBindguard buffer_guard(GL_READ_BUFFER, buffer.name());
  TextureBindguard texture_guard(_target, name());
  GL_CALL(glCopyTexSubImage2D(
    _target,
    level,
    xoffset,
    yoffset,
    x,
    y,
    w,
    h
  ));
}



} // namespace gl
