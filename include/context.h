#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"
#include "generated_object.h"
#include "framebuffer.h"

#include <functional>

namespace gl {

struct OS_Bridge {
  std::function<void(void*)> make_current;
};




class Pipeline;
class Buffer;
class Program;
class Framebuffer;
class Texture;
class VertexArray;


class Context : public BasicFramebuffer {
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


  public: // BasicFramebuffer
    void clear(GLenum mask) override;
    void draw(Program const&, VertexArray const&, GLenum mode, size_t count, size_t first = 0) override;

  public:
    void color_mask(color const&);
    void color_mask(GLuint buf, color const&);

    template<GLenum>
    void cull_face();

    template<GLenum> void enable();
    template<GLenum> void disable();
    template<GLenum> bool is_enabled() const;

    template<typename T>
    void clear_depth(T depth);

    void clear_stencil(int);

    void line_width(float);

    template<typename T, GLenum param>
    T get() const {
      return get<T>(param);
    }

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
