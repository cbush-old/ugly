#include "texture.h"
#include "buffer.h"

namespace gl {


#define OFFSETS1 xoffset
#define OFFSETS2 OFFSETS1, yoffset
#define OFFSETS3 OFFSETS2, zoffset

#define DIMENSIONS1 desc.width
#define DIMENSIONS2 DIMENSIONS1, desc.height
#define DIMENSIONS3 DIMENSIONS2, desc.depth


#define IMPLEMENT_IMAGE_OR_UNPACK(ND, DATA_OR_OFFSET, ...) \
  TextureBindguard guard(_bind_target, name()); \
  GL_CALL(glTexImage##ND ( \
    _target, \
    level, \
    _internal_format, \
    /* dimensions */ __VA_ARGS__, \
    /* border */ 0, \
    desc.format, \
    desc.type, \
    DATA_OR_OFFSET \
  ))

#define IMPLEMENT_IMAGE(ND, DIMENSIONS) \
  IMPLEMENT_IMAGE_OR_UNPACK(ND, desc.data, DIMENSIONS)

#define IMPLEMENT_SUB_IMAGE_OR_UNPACK(ND, DATA_OR_OFFSET, ...) \
  TextureBindguard guard(_bind_target, name()); \
  GL_CALL(glTexSubImage##ND ( \
    _target, \
    level, \
    /* OFFSETS, DIMENSIONS */ __VA_ARGS__, \
    desc.format, \
    desc.type, \
    DATA_OR_OFFSET \
  ))

#define IMPLEMENT_SUBIMAGE(ND, OFFSETS, DIMENSIONS) \
  IMPLEMENT_SUB_IMAGE_OR_UNPACK(ND, desc.data, OFFSETS, DIMENSIONS)

#define IMPLEMENT_UNPACK(ND, DIMENSIONS) \
  BufferBindguard buffer_guard(GL_PIXEL_UNPACK_BUFFER, buffer.name()); \
  IMPLEMENT_IMAGE_OR_UNPACK(ND, (void const*)offset, DIMENSIONS)

#define IMPLEMENT_SUB_UNPACK(ND, OFFSETS, DIMENSIONS) \
  BufferBindguard buffer_guard(GL_PIXEL_UNPACK_BUFFER, buffer.name()); \
  IMPLEMENT_SUB_IMAGE_OR_UNPACK(ND, (void const*)offset, OFFSETS, DIMENSIONS)


#define IMPLEMENT_COPY(ND, ...) \
  BufferBindguard buffer_guard(GL_COPY_READ_BUFFER, buffer.name()); \
  TextureBindguard texture_guard(_bind_target, name()); \
  GL_CALL(glCopyTexImage##ND( \
    _target, \
    level, \
    _internal_format, \
    __VA_ARGS__, \
    /* border */ 0 \
  ))

#define IMPLEMENT_SUBCOPY(ND, OFFSETS, ...) \
  BufferBindguard buffer_guard(GL_COPY_READ_BUFFER, buffer.name()); \
  TextureBindguard texture_guard(_bind_target, name()); \
  GL_CALL(glCopyTexSubImage##ND( \
    _target, \
    level, \
    OFFSETS, \
    __VA_ARGS__ \
  ))



// Non-cubemaps do not need to distinguish binding target to function target.
#define _bind_target _target


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




Texture1D::Texture1D(GLenum internal_format /* = GL_RGBA */)
  : Texture(GL_TEXTURE_1D, internal_format)
  {}


void Texture1D::image(int level, ImageDesc1D const& desc) {
  IMPLEMENT_IMAGE(1D, DIMENSIONS1);
}

void Texture1D::subimage(int level, unsigned xoffset, ImageDesc1D const& desc) {
  IMPLEMENT_SUBIMAGE(1D, OFFSETS1, DIMENSIONS1);
}

void Texture1D::image(int level, Buffer const& buffer, ImageDesc1D const& desc, size_t offset) {
  IMPLEMENT_UNPACK(1D, DIMENSIONS1);
}

void Texture1D::subimage(int level, unsigned xoffset, Buffer const& buffer, ImageDesc1D const& desc, size_t offset) {
  IMPLEMENT_SUB_UNPACK(1D, OFFSETS1, DIMENSIONS1);
}

void Texture1D::copy(int level, Buffer const& buffer, int x, int y, GLsizei w) {
  IMPLEMENT_COPY(1D, x, y, w);
}

void Texture1D::subcopy(int level, unsigned xoffset, Buffer const& buffer, int x, int y, GLsizei w) {
  IMPLEMENT_SUBCOPY(1D, OFFSETS1, x, y, w);
}




Texture2D::Texture2D(GLenum internal_format /* = GL_RGBA */)
  : Texture(GL_TEXTURE_2D, internal_format)
  {}


void Texture2D::image(int level, ImageDesc2D const& desc) {
  IMPLEMENT_IMAGE(2D, DIMENSIONS2);
}

void Texture2D::subimage(int level, unsigned xoffset, unsigned yoffset, ImageDesc2D const& desc) {
  IMPLEMENT_SUBIMAGE(2D, OFFSETS2, DIMENSIONS2);
}

void Texture2D::image(int level, Buffer const& buffer, ImageDesc2D const& desc, size_t offset) {
  IMPLEMENT_UNPACK(2D, DIMENSIONS2);
}

void Texture2D::subimage(int level, unsigned xoffset, unsigned yoffset, Buffer const& buffer, ImageDesc2D const& desc, size_t offset) {
  IMPLEMENT_SUB_UNPACK(2D, OFFSETS2, DIMENSIONS2);
}

void Texture2D::copy(int level, Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  IMPLEMENT_COPY(2D, x, y, w, h);
}

void Texture2D::subcopy(int level, unsigned xoffset, unsigned yoffset,
  Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  IMPLEMENT_SUBCOPY(2D, OFFSETS2, x, y, w, h);
}






Texture3D::Texture3D(GLenum internal_format /* = GL_RGBA */)
  : Texture(GL_TEXTURE_3D, internal_format)
  {}


void Texture3D::image(int level, ImageDesc3D const& desc) {
  IMPLEMENT_IMAGE(3D, DIMENSIONS3);
}

void Texture3D::subimage(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset, ImageDesc3D const& desc) {
  IMPLEMENT_SUBIMAGE(3D, OFFSETS3, DIMENSIONS3);
}

void Texture3D::image(int level, Buffer const& buffer, ImageDesc3D const& desc, size_t offset) {
  IMPLEMENT_UNPACK(3D, DIMENSIONS3);
}

void Texture3D::subimage(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset, Buffer const& buffer, ImageDesc3D const& desc, size_t offset) {
  IMPLEMENT_SUB_UNPACK(3D, OFFSETS3, DIMENSIONS3);
}

void Texture3D::subcopy(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset,
  Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  IMPLEMENT_SUBCOPY(3D, OFFSETS3, x, y, w, h);
}



#undef _bind_target
GLenum const Cubemap::Face::_bind_target = GL_TEXTURE_CUBE_MAP;

Cubemap::Face::Face(GLuint name, GLenum target, GLenum internal_format)
  : Texture(name, target, internal_format)
  {}

void Cubemap::Face::image(int level, ImageDesc2D const& desc) {
  IMPLEMENT_IMAGE(2D, DIMENSIONS2);
}

void Cubemap::Face::subimage(int level, unsigned xoffset, unsigned yoffset, ImageDesc2D const& desc) {
  IMPLEMENT_SUBIMAGE(2D, OFFSETS2, DIMENSIONS2);
}

void Cubemap::Face::image(int level, Buffer const& buffer, ImageDesc2D const& desc, size_t offset) {
  IMPLEMENT_UNPACK(2D, DIMENSIONS2);
}

void Cubemap::Face::subimage(int level, unsigned xoffset, unsigned yoffset, Buffer const& buffer, ImageDesc2D const& desc, size_t offset) {
  IMPLEMENT_SUB_UNPACK(2D, OFFSETS2, DIMENSIONS2);
}

void Cubemap::Face::copy(int level, Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  IMPLEMENT_COPY(2D, x, y, w, h);
}

void Cubemap::Face::subcopy(int level, unsigned xoffset, unsigned yoffset,
  Buffer const& buffer, int x, int y, GLsizei w, GLsizei h) {
  IMPLEMENT_SUBCOPY(2D, OFFSETS2, x, y, w, h);
}


Cubemap::Cubemap(GLenum internal_format /* = GL_RGBA */)
  : Texture(GL_TEXTURE_CUBE_MAP, internal_format)
  , _faces {
    { name(), GL_TEXTURE_CUBE_MAP_POSITIVE_X, internal_format },
    { name(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, internal_format },
    { name(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, internal_format },
    { name(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, internal_format },
    { name(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, internal_format },
    { name(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, internal_format }
  }
  {}


} // namespace gl
