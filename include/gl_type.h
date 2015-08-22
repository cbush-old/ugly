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
#include <sstream>

namespace gl {

class exception {
  public:
    exception(std::string what): _what(what) {}
    exception(const char* what): _what(what) {}

    template<typename... T>
    exception(const char* fmt, T const&... args) {
      std::stringstream ss;
      format(ss, fmt, args...);
      _what = ss.str();
    }

  public:
    const char* what() const { return _what.c_str(); }

  protected:
    void format(std::ostream&, const char* fmt);

    template<typename T, typename... Args>
    void format(std::ostream&, const char* fmt, T const&, Args const&... args);

  protected:
    std::string _what;

};

inline void exception::format(std::ostream& os, const char* fmt) {
  os << fmt;
}

template<typename T, typename... Args>
inline void exception::format(std::ostream& os, const char* fmt, T const& value, Args const&... args) {
  while (*fmt) {  
    if (*fmt == '%') {
      if (fmt[1] == '%') {
        ++fmt;
      } else {
        os << value;
        fmt += 2;
        format(os, fmt, args...);
        return;
      }
    }
    os << *fmt;
    ++fmt;
  }
}


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
      throw gl::exception("gl call " #x " failed: %d", error); \
    } \
  }

#define GL_VALIDATE(Type, name) {\
  GL_CALL(bool valid = glIs##Type(name)) \
  if (!valid) { throw gl::exception("%u is not a " #Type "!", name); } \
  else { logv("named object %u is a " #Type, name); } \
  }


class IContext;
class BaseContext;

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


class Shader;

class IProgram {
  public:
    virtual ~IProgram() =0;

  public:
    /**
     * @brief Attach a Shader to the Program.
     * @param shader  The Shader to attach.
     **/
    virtual void attach(Shader const& shader) =0;

    /**
     * @brief Detach the given Shader from the Program.
     * @param shader  The Shader to detach.
     **/
    virtual void detach(Shader const& shader) =0;

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

class IController {
  public:
    virtual ~IController() {}

  public:
    virtual void activate(IContext&) =0;
    virtual void deactivate(IContext&) =0;
};

class IContext {
  public:
    virtual ~IContext() {}

  public:
    /**
     * @brief attach a Controller to the Context
     **/
    virtual void attach(IController&) =0;
    virtual void detach(IController&) =0;

  public:
    /**
     * @brief retrieve the major OpenGL version
     **/
    virtual unsigned major_version() const =0;

    /**
     * @brief retrieve the minor OpenGL version
     **/
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

  public:
    virtual void clear() =0;
    virtual void clear(GLbitfield mask) =0;


  protected:
    friend class ContextAssociatedObject;
    virtual void add(ContextAssociatedObject*) =0;
    virtual void remove(ContextAssociatedObject*) =0;

};


} // namespace gl

#endif
