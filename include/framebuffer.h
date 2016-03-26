#ifndef UGLY_FRAMEBUFFER_H
#define UGLY_FRAMEBUFFER_H

#include "gl_type.h"
#include "generated_object.h"

namespace gl {


class Texture1D;
class Texture2D;
class Texture3D;
class Cubemap;
class Renderbuffer;
class VertexArray;


class BasicFramebuffer {
  public:
    void clear_color(float r, float g, float b, float a = 1.f);
    void clear_color(color c);
    void viewport(float x, float y, float w, float h);
    void viewport(Viewport viewport);
    Viewport const& viewport() const;

  public:
    virtual void clear(GLenum mask) =0;
    virtual void draw(Program const&, VertexArray const&, GLenum mode, size_t count, size_t first = 0) =0;
  
    /**
     * @brief draw using the given VertexArray object's 'mode' and 'count'.
     **/
    virtual void draw(Program const&, VertexArray const&);
  
    virtual void draw_instanced(Program const&, VertexArray const&, size_t instance_count, GLenum mode, size_t count, size_t first = 0) =0;
  
  public:
    virtual void draw_buffer(GLenum buffer);

  protected:
    Viewport _viewport;
    color _clear_color;

};

class Framebuffer
  : public GeneratedObject<glGenFramebuffers, glDeleteFramebuffers>
  , public BasicFramebuffer
{
  public:
    void texture(GLenum attachment, Texture1D const& texture, int level = 0);
    void texture(GLenum attachment, Texture2D const& texture, int level = 0);
    void texture(GLenum attachment, Cubemap const& texture, int level);
    void texture(GLenum attachment, Texture3D const& texture, int level, int layer);
  
  public:
    void renderbuffer(GLenum attachment, Renderbuffer const& renderbuffer);

  public:
    void clear(GLenum mask) override;
    void draw(Program const& program, VertexArray const&, GLenum mode, size_t count, size_t first = 0) override;
    void draw_instanced(Program const&, VertexArray const&, size_t instance_count, GLenum mode, size_t count, size_t first = 0) override;

  public:
    void draw_buffer(GLenum buffer) override;

  public:
    bool is_complete() const;
    GLenum status() const;
    const char* status_str() const;

};

} // namespace gl

#endif
