#ifndef UGLY_TEXTURE_H
#define UGLY_TEXTURE_H

#include "gl_type.h"
#include "generated_object.h"

#include <array>
#include <memory>

namespace gl {


class Buffer;


struct ImageDesc {
  public:
    ImageDesc() {}
    ImageDesc(GLsizei width, void const* data): data(data), width(width) {}

  public:
    void const* data { nullptr };
    GLenum format { GL_RGBA };
    GLenum type { GL_UNSIGNED_INT_8_8_8_8 };
    GLsizei width { 0 };
};

using ImageDesc1D = ImageDesc;

struct ImageDesc2D : public ImageDesc {
  public:
    ImageDesc2D() {}
    ImageDesc2D(GLsizei width, GLsizei height, void const* data)
      : ImageDesc(width, data), height(height)
      {}

  public:
    GLsizei height { 0 };
};

struct ImageDesc3D : public ImageDesc2D {
  public:
    ImageDesc3D() {}
    ImageDesc3D(GLsizei width, GLsizei height, GLsizei depth, void const* data)
      : ImageDesc2D(width, height, data), depth(depth)
      {}
  
  public:
    GLsizei depth { 0 };
};


struct ParamVariant {
  ParamVariant(GLint i): i(i), type(I) {}
  ParamVariant(GLfloat f): f(f), type(F) {}
  ParamVariant(GLfloat const* inf4): type(FV) {
    std::memcpy(f4, inf4, 4);
  }
  
  ParamVariant(ParamVariant const& o): type(o.type) {
    std::memcpy(f4, o.f4, 4);
  }
  
  ParamVariant& operator=(ParamVariant const& o) {
    type = o.type;
    std::memcpy(f4, o.f4, 4);
    return *this;
  }

  union {
    GLint i;
    GLfloat f;
    GLfloat f4[4];
  };

  enum Type {
    I, F, FV
  } type;

};


using TextureParams = std::map<GLenum, ParamVariant>;



class Texture : public GeneratedObject<glGenTextures, glDeleteTextures> {
  protected:
    explicit Texture(GLenum target, TextureParams const& params, GLenum internal_format);
    explicit Texture(GLenum target, GLenum internal_format);
    explicit Texture(GLuint name, GLenum target, GLenum internal_format);

  public:
    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;
    ~Texture();

  public:
    void parameter(GLenum pname, float);
    void parameter(GLenum pname, int);
    void parameter(GLenum pname, GLfloat const*);
    void parameter(GLenum pname, GLint const*);
  
  public:
    GLenum target() const;

  protected:
    GLenum const _target;
    GLenum _internal_format;

};



class Texture1D : public Texture {
  public:
    explicit Texture1D(GLenum internal_format = GL_RGBA);
    Texture1D(TextureParams const& params, GLenum internal_format = GL_RGBA);

  public:
    void storage(GLsizei levels, GLsizei w);

    void image(int level, ImageDesc1D const&);

    /**
     * @brief specify image data using the given buffer the pixel unpack buffer.
     **/
    void image(int level, Buffer const&, ImageDesc1D const&, size_t offset);

    void subimage(int level, unsigned xoffset, ImageDesc1D const&);

    void subimage(int level, unsigned xoffset, Buffer const&, ImageDesc1D const&, size_t offset);

    void copy(int level, Buffer const& buffer, int x, int y, GLsizei w);

    void subcopy(int level, unsigned xoffset, Buffer const&, int x, int y, GLsizei w);
  
};


class Texture2D : public Texture {
  public:
    explicit Texture2D(GLenum internal_format = GL_RGBA);
    Texture2D(TextureParams const& params, GLenum internal_format = GL_RGBA);
  
  protected:
    Texture2D(GLuint name, GLenum target, GLenum internal_format);

  public:
    void storage(GLsizei levels, GLsizei w, GLsizei h);

    void image(int level, ImageDesc2D const&);

    /**
     * @brief specify image data using the given buffer
     **/
    void image(int level, Buffer const&, ImageDesc2D const&, size_t offset);

    void subimage(int level, unsigned xoffset, unsigned yoffset, ImageDesc2D const&);

    void subimage(int level, unsigned xoffset, unsigned yoffset, Buffer const&, ImageDesc2D const&, size_t offset);

    void copy(int level, Buffer const& buffer, int x, int y, GLsizei w, GLsizei h);

    void subcopy(int level, unsigned xoffset, unsigned yoffset, Buffer const&, int x, int y, GLsizei w, GLsizei h);
  
};


class Texture3D : public Texture {
  public:
    explicit Texture3D(GLenum internal_format = GL_RGBA);
    Texture3D(TextureParams const& params, GLenum internal_format = GL_RGBA);

  public:
    void storage(GLsizei levels, GLsizei w, GLsizei h, GLsizei d);

    void image(int level, ImageDesc3D const&);

    /**
     * @brief specify image data using the given buffer the pixel unpack buffer.
     **/
    void image(int level, Buffer const&, ImageDesc3D const&, size_t offset);

    void subimage(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset, ImageDesc3D const&);

    void subimage(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset, Buffer const&, ImageDesc3D const&, size_t offset);

  public:
    void subcopy(int level, unsigned xoffset, unsigned yoffset, unsigned zoffset,
      Buffer const&, int x, int y, GLsizei w, GLsizei h);

};



class Cubemap : public Texture {
  public:
    enum FaceIndex {
      POSITIVE_X = 0,
      POSITIVE_Y,
      POSITIVE_Z,
      NEGATIVE_X,
      NEGATIVE_Y,
      NEGATIVE_Z,
    };

  private:
    class Face : public Texture {
      private:
        static const GLenum _bind_target;

      public:
        Face(GLuint name, GLenum target, GLenum internal_format = GL_RGBA);
      
      public:
        void image(int level, ImageDesc2D const& desc);
        void image(int level, Buffer const&, ImageDesc2D const&, size_t offset);
        void subimage(int level, unsigned xoffset, unsigned yoffset, ImageDesc2D const&);
        void subimage(int level, unsigned xoffset, unsigned yoffset, Buffer const&, ImageDesc2D const&, size_t offset);
        void copy(int level, Buffer const& buffer, int x, int y, GLsizei w, GLsizei h);
        void subcopy(int level, unsigned xoffset, unsigned yoffset, Buffer const&, int x, int y, GLsizei w, GLsizei h);

    } _faces[6];

  public:
    explicit Cubemap(GLenum internal_format = GL_RGBA);
    Cubemap(TextureParams const& params, GLenum internal_format = GL_RGBA);

  public:
    Face& operator[](FaceIndex i);
    Face const& operator[](FaceIndex i) const;

  public:
    void storage(GLsizei levels, GLsizei w, GLsizei h);

};



using TextureRef = std::shared_ptr<Texture>;



} // namespace gl

#endif
