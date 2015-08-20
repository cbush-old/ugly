#ifndef GL_TYPE_H
#define GL_TYPE_H

#include "log.h"

#include <gl3.h>
#include <gl3ext.h>

#include <map>
#include <thread>

namespace gl {

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
      throw; \
    } \
  }

#define DECL_GL_OBJECT(Class) \
  friend class Context; \
  private: GLuint _name { 0 }; \
           Class(); \
           Class(GLuint); \
  public: ~Class(); \
           Class(Class const&) = delete; \
           Class& operator=(Class const&) = delete;

#define DEFINE_GL_OBJECT(Class, Plural) \
  Class::Class() { GL_CALL(glGen##Plural(1, &_name)); } \
  Class::Class(GLuint name): _name(name) {} \
  Class::~Class() { try { GL_CALL(glDelete##Plural(1, &_name)); } catch(...){} }


class IContext;

class ContextAssociatedObject {
  public:
    ContextAssociatedObject(IContext& context);
    virtual ~ContextAssociatedObject() =0;

  protected:
    IContext& _context;
};


class IContextConfigurator {
  public:
    virtual ~IContextConfigurator() {}

  public:
    virtual void activate() =0;
    virtual void deactivate() =0;

};

class IViewport : public IContextConfigurator {
  public:
    virtual ~IViewport() =0;
};

class IBlend : public IContextConfigurator {
  public:
    virtual ~IBlend() =0;

};

class ICullface : public IContextConfigurator {
  public:
    virtual ~ICullface() =0;
};

class IDepth : public IContextConfigurator {
  public:
    virtual ~IDepth() =0;
};

class IColor : public IContextConfigurator {
  public:
    virtual ~IColor() =0;
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

};

inline IProgram::~IProgram() {}


class IBuffer {
  public:
    virtual ~IBuffer() =0;

};


class IContext {
  public:
    virtual ~IContext() {}

  public:
    virtual void set_viewport(IViewport&) =0;
    virtual void set_blend(IBlend&) =0;
    virtual void set_cullface(ICullface&) =0;
    virtual void set_depth(IDepth&) =0;
    virtual void set_color(IColor&) =0;

  public:
    /**
     * @brief Bind a Program to the Context.
     **/
    virtual void bind(IProgram const&) =0;

    /**
     * @brief Unbind a Program from the Context.
     **/
    virtual void unbind(IProgram const&) =0;

  public:
    /**
     * @brief Make this Context the current Context of this thread.
     **/
    virtual void make_current() =0;

    /**
     * @brief Check whether the Context is current on this thread.
     * @return true if the Context is current on this thread.
     **/
    virtual bool current() const =0;


  protected:
    friend class ContextAssociatedObject;
    virtual void add(ContextAssociatedObject*) =0;
    virtual void remove(ContextAssociatedObject*) =0;

};


} // namespace gl

#endif
