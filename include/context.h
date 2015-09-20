#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"


namespace gl {

class Pipeline;
class Buffer;
class Framebuffer;

class Context {
  public:
    Context();

  public:
    virtual ~Context() =0;
    Context(Context const&) = delete;
    Context& operator=(Context const&) = delete;

  public: // OPENGL VERSION INFO
    /**
     * @brief retrieve the major OpenGL version
     **/
    unsigned major_version() const;

    /**
     * @brief retrieve the minor OpenGL version
     **/
    unsigned minor_version() const;


  public: // CURRENT CONTEXT
    /**
     * @brief make this Context the current Context of this thread.
     **/
    void make_current();

    /**
     * @brief check whether the Context is current on this thread.
     * @return true if the Context is current on this thread.
     **/
    bool current() const;


  public:
    void bind(Pipeline&);

    template<GLenum target>
    void bind(Framebuffer&);


  public:
    void clear();
    void clear(GLbitfield mask);

  public:
    /**
     * @brief set the color buffer value
     **/
    void clear_color(color const&);
    void color_mask(color const&);
    void color_mask(GLuint buf, color const&);

    template<GLenum>
    void cull_face();

    template<GLenum> void enable();
    template<GLenum> void disable();

    template<GLenum> void enable(GLuint);
    template<GLenum> void disable(GLuint);

    template<GLenum> bool is_enabled();
    template<GLenum> bool is_enabled(GLuint);

    template<GLenum>
    void draw_arrays(int first, GLsizei count);

    template<typename T>
    void clear_depth(T depth);

    void clear_stencil(int);

    /**
     * @brief bind buffer to target
     **/
    template<GLenum target>
    void bind(Buffer& buffer);


  protected:
    class Context_impl* _impl { nullptr };

};


#define INSTANTIATE_ENABLE(MODE) \
  extern template void Context::enable< MODE >(); \
  extern template void Context::disable< MODE >(); \
  extern template bool Context::is_enabled<MODE>();

INSTANTIATE_ENABLE(GL_BLEND);
INSTANTIATE_ENABLE(GL_COLOR_LOGIC_OP);
INSTANTIATE_ENABLE(GL_CULL_FACE);
INSTANTIATE_ENABLE(GL_DEPTH_CLAMP);
INSTANTIATE_ENABLE(GL_DEPTH_TEST);
INSTANTIATE_ENABLE(GL_DITHER);
INSTANTIATE_ENABLE(GL_FRAMEBUFFER_SRGB);
INSTANTIATE_ENABLE(GL_LINE_SMOOTH);
INSTANTIATE_ENABLE(GL_MULTISAMPLE);
INSTANTIATE_ENABLE(GL_POLYGON_OFFSET_FILL);
INSTANTIATE_ENABLE(GL_POLYGON_OFFSET_LINE);
INSTANTIATE_ENABLE(GL_POLYGON_OFFSET_POINT);
INSTANTIATE_ENABLE(GL_POLYGON_SMOOTH);
INSTANTIATE_ENABLE(GL_PRIMITIVE_RESTART);
INSTANTIATE_ENABLE(GL_RASTERIZER_DISCARD);
INSTANTIATE_ENABLE(GL_SAMPLE_ALPHA_TO_COVERAGE);
INSTANTIATE_ENABLE(GL_SAMPLE_ALPHA_TO_ONE);
INSTANTIATE_ENABLE(GL_SAMPLE_COVERAGE);
INSTANTIATE_ENABLE(GL_SAMPLE_SHADING);
INSTANTIATE_ENABLE(GL_SAMPLE_MASK);
INSTANTIATE_ENABLE(GL_SCISSOR_TEST);
INSTANTIATE_ENABLE(GL_STENCIL_TEST);
INSTANTIATE_ENABLE(GL_TEXTURE_CUBE_MAP_SEAMLESS);
INSTANTIATE_ENABLE(GL_PROGRAM_POINT_SIZE);
#if GL_4_5
INSTANTIATE_ENABLE(GL_DEBUG_OUTPUT);
INSTANTIATE_ENABLE(GL_DEBUG_OUTPUT_SYNCHRONOUS);
INSTANTIATE_ENABLE(GL_PRIMITIVE_RESTART_FIXED_INDEX);
#endif

#undef INSTANTIATE_ENABLE


extern template void Context::draw_arrays<GL_POINTS>(int, GLsizei);
extern template void Context::draw_arrays<GL_LINE_STRIP>(int, GLsizei);
extern template void Context::draw_arrays<GL_LINE_LOOP>(int, GLsizei);
extern template void Context::draw_arrays<GL_LINES>(int, GLsizei);
extern template void Context::draw_arrays<GL_LINE_STRIP_ADJACENCY>(int, GLsizei);
extern template void Context::draw_arrays<GL_LINES_ADJACENCY>(int, GLsizei);
extern template void Context::draw_arrays<GL_TRIANGLE_STRIP>(int, GLsizei);
extern template void Context::draw_arrays<GL_TRIANGLE_FAN>(int, GLsizei);
extern template void Context::draw_arrays<GL_TRIANGLES>(int, GLsizei);
extern template void Context::draw_arrays<GL_TRIANGLE_STRIP_ADJACENCY>(int, GLsizei);
extern template void Context::draw_arrays<GL_TRIANGLES_ADJACENCY>(int, GLsizei);
extern template void Context::draw_arrays<GL_PATCHES>(int, GLsizei);




/**
 * @brief a Context to be used when Context activity will always happen on the same thread.
 **/
class MonoContext : public Context {
  public:
    MonoContext(void*);
    ~MonoContext() override;
};


/**
 * @brief a Context to be used when Context activity might happen from multiple threads.
 *
 * Each MultiContext is tied to the thread on which it was created, and cannot be used
 * from any other thread. However, you can create another MultiContext on a different
 * thread.
 **/
class MultiContext : public Context {
  public:
    MultiContext(void*);
    ~MultiContext() override;
};


}


#endif
