#ifndef GL_TYPE_H
#define GL_TYPE_H

#include "log.h"

#include <gl3.h>
#include <gl3ext.h>

#include <map>
#include <thread>
#include <vector>
#include <string>
#include <stdexcept>

namespace gl {

class exception {
  public:
    exception(std::string what): _what(what) {}

  public:
    const char* what() const { return _what.c_str(); }

  protected:
    std::string _what;

};

template<typename T>
struct vec2_t {
  union { T x, low, near; };
  union { T y, high, far; };
};

template<typename T>
struct vec4_t {
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };
};

using range = vec2_t<float>;
using vec4 = vec4_t<float>;
using color = vec4;

using attribute = GLint;


#define GL_CALL(x) \
    x; {\
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
      loge(#x " failed: %d", error); \
      throw gl::exception("gl call " #x " failed"); \
    } \
  }


class IContext;

class ContextAssociatedObject {
  public:
    ContextAssociatedObject(IContext& context);
    virtual ~ContextAssociatedObject() =0;

  protected:
    IContext& _context;
};




template<void(*glGenFunc)(GLsizei, GLuint*), void(*glDeleteFunc)(GLsizei, GLuint const*)>
class GeneratedObject {
  public:
    GeneratedObject();
    GeneratedObject(GLuint name);
    virtual ~GeneratedObject() =0;

  protected:
    GLuint _name;

};


class IShader {
  public:
    virtual ~IShader() =0;

  public: // Shader source functions
    virtual void set_source(std::string const& source) =0;
    virtual void set_source(std::vector<std::string> const& sources) =0;
    virtual std::string get_source() const =0;

  public: // Shader info
    virtual GLuint name() const =0;
    virtual GLenum type() const =0;
    virtual bool deleted() const =0;
    virtual bool compiled() const =0;
    virtual unsigned source_length() const =0;

  public: // Shader compilation
    virtual void compile() =0;

};

inline IShader::~IShader() {}

class IProgram {
  public:
    virtual ~IProgram() =0;

  public:
    /**
     * @brief Attach a Shader to the Program.
     * @param shader  The Shader to attach.
     **/
    virtual void attach(IShader const& shader) =0;

    /**
     * @brief Detach the given Shader from the Program.
     * @param shader  The Shader to detach.
     **/
    virtual void detach(IShader const& shader) =0;

    /**
     * @brief Link the Program.
     * 
     * Call this after all the shaders are attached.
     **/
    virtual void link() =0;

    /**
     * @brief install the Program as part of the given Context rendering state.
     **/
    virtual void use(IContext const& context) =0;


};

inline IProgram::~IProgram() {}



enum BufferIndex {
  BUFFER_INDEX_ARRAY = 0,
  // BUFFER_INDEX_ATOMIC_COUNTER, // 4.2+
  BUFFER_INDEX_COPY_READ,
  BUFFER_INDEX_COPY_WRITE,
  // BUFFER_INDEX_DISPATCH_INDIRECT, // 4.3+
  BUFFER_INDEX_DRAW_INDIRECT,
  BUFFER_INDEX_ELEMENT_ARRAY,
  BUFFER_INDEX_PIXEL_PACK,
  BUFFER_INDEX_PIXEL_UNPACK,
  // BUFFER_INDEX_QUERY, // 4.4+
  // BUFFER_INDEX_SHADER_STORAGE, // 4.3+
  BUFFER_INDEX_TEXTURE,
  BUFFER_INDEX_TRANSFORM_FEEDBACK,
  BUFFER_INDEX_UNIFORM,
  BUFFER_INDEX_MAX
};


class IBuffer {
  public:
    virtual ~IBuffer() =0;

  public:
    /**
     * @brief bind a Buffer to the target of the current Context.
     * @param target  the target to bind the buffer to
     **/
     virtual void bind(BufferIndex target);

    /**
     * @brief unbind the target
     **/
     virtual void unbind(BufferIndex);


};


class IContext {
  public:
    class IController {
      public:
        virtual ~IController() {}

      public:
        virtual void activate(IContext&) =0;
        virtual void deactivate(IContext&) =0;
    };

  public:
    virtual ~IContext() {}

  public:
    /**
     * @brief attach a Controller to the Context
     **/
    virtual void attach(IController&) =0;
    virtual void detach(IController&) =0;

  public:
    virtual unsigned major_version() const =0;
    virtual unsigned minor_version() const =0;

  public:
    /**
     * @brief make this Context the current Context of this thread.
     **/
    virtual void make_current() =0;

    /**
     * @brief check whether the Context is current on this thread.
     * @return true if the Context is current on this thread.
     **/
    virtual bool current() const =0;

    /**
     * @brief Called when this Context is current and another Context takes 
     * @note "I used to be 'with it'. Then they changed what 'it' was. Now what
     *        I'm 'with' isn't 'it', and what is 'it' seems weird and scary to me.
     *        It'll happen to you!"
     **/
    virtual void on_made_not_current() =0;


  protected:
    friend class ContextAssociatedObject;
    virtual void add(ContextAssociatedObject*) =0;
    virtual void remove(ContextAssociatedObject*) =0;

};


} // namespace gl

#endif
