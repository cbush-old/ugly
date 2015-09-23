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

    template<GLenum> bool is_enabled() const;
    template<GLenum> bool is_enabled(GLuint) const;

    template<GLenum>
    void draw_arrays(int first, GLsizei count);

    template<typename T>
    void clear_depth(T depth);

    void clear_stencil(int);

    void line_width(float);

    template<typename T, GLenum param>
    T get() const {
      return get<T>(param);
    }

    /**
     * @brief bind buffer to target
     **/
    template<GLenum target>
    void bind(Buffer& buffer);



  public: // Texture
    void active_texture(size_t i);
    size_t active_texture() const;




  private:
    template<typename T>
    T get(GLenum) const;

    template<GLenum param, GLenum size_key>
    std::vector<int> get() const;

  protected:
    class Context_impl* _impl { nullptr };

};


template<> bool Context::get<bool>(GLenum) const;
template<> int Context::get<int>(GLenum) const;
template<> unsigned Context::get<unsigned>(GLenum) const;
template<> int64_t Context::get<int64_t>(GLenum) const;
template<> GLenum Context::get<GLenum>(GLenum) const;
template<> range Context::get<range>(GLenum) const;
template<> color Context::get<color>(GLenum) const;

#include "context_instantiations.inl"


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
