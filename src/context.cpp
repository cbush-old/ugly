#include "context.h"

#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

namespace gl {

/*
// Lookup table for BufferIndex to GL_TARGET_BUFFER enums.
// Obviously, BufferIndex order must match this table order.
// Have you updated either table recently? Do the orders 
// still match? Well, do they? Did you double check...?!
static const GLenum buffer_target[BUFFER_INDEX_MAX] {
  GL_ARRAY_BUFFER,
  // GL_ATOMIC_COUNTER_BUFFER,
  GL_COPY_READ_BUFFER,
  GL_COPY_WRITE_BUFFER,
  // GL_DISPATCH_INDIRECT_BUFFER,
  GL_DRAW_INDIRECT_BUFFER,
  GL_ELEMENT_ARRAY_BUFFER,
  GL_PIXEL_PACK_BUFFER,
  GL_PIXEL_UNPACK_BUFFER,
  // GL_QUERY_BUFFER,
  // GL_SHADER_STORAGE_BUFFER,
  GL_TEXTURE_BUFFER,
  GL_TRANSFORM_FEEDBACK_BUFFER,
  GL_UNIFORM_BUFFER,
};
*/


class Context_impl {
  public:
    Context_impl(void *);
    virtual ~Context_impl() =0;


  public:
    virtual void make_current() =0;
    virtual bool current() const =0;
    void on_made_not_current();

  public:
    std::set<ContextAssociatedObject const*> _associated_objects;
    GLbitfield _clear_mask { GL_COLOR_BUFFER_BIT };

  protected:
    void *_handle { nullptr };
};


class MonoContext_impl : public Context_impl {
  private:
    static MonoContext_impl* current_context;

  public:
    MonoContext_impl(void *);
    ~MonoContext_impl();

  public:
    void make_current() override;
    bool current() const override;

};


class MultiContext_impl : public Context_impl {
  private:
    static std::map<std::thread::id, MultiContext_impl*> current_context;
    static std::recursive_mutex current_context_lock;

  public:
    MultiContext_impl(void *);
    ~MultiContext_impl();

  public:
    void make_current() override;
    bool current() const override;

  private:
    std::thread::id _thread_id;

};



MonoContext_impl* MonoContext_impl::current_context { 0 };

std::map<std::thread::id, MultiContext_impl*> MultiContext_impl::current_context;
std::recursive_mutex MultiContext_impl::current_context_lock;




template<typename T, void(*GET)(GLenum, T*)>
inline T get(BaseContext const& context, GLenum param) {
  if (!context.current()) {
    throw gl::exception("can't get param from inactive context");
  }
  T rv;
  GL_CALL(GET(param, &rv));
  return rv;
}

template<typename T>
inline T get(BaseContext const& context, GLenum param);

template<>
inline int get<int>(BaseContext const& context, GLenum param) {
  return get<GLint, glGetIntegerv>(context, param);
}

template<>
inline int64_t get<int64_t>(BaseContext const& context, GLenum param) {
  return get<GLint64, glGetInteger64v>(context, param);
}

template<>
inline bool get<bool>(BaseContext const& context, GLenum param) {
  return get<GLboolean, glGetBooleanv>(context, param);
}

template<>
inline double get<double>(BaseContext const& context, GLenum param) {
  return get<GLdouble, glGetDoublev>(context, param);
}

template<>
inline float get<float>(BaseContext const& context, GLenum param) {
  return get<GLfloat, glGetFloatv>(context, param);
}



unsigned BaseContext::major_version() const {
  return (unsigned)get<int>(*this, GL_MAJOR_VERSION);
}

unsigned BaseContext::minor_version() const {
  return (unsigned)get<int>(*this, GL_MINOR_VERSION);
}

BaseContext::~BaseContext() {
  delete _impl;
}

void Context_impl::on_made_not_current() {}


void BaseContext::clear() {
  GL_CALL(glClear(_impl->_clear_mask));
}

void BaseContext::clear(GLbitfield mask) {
  GL_CALL(glClear(mask));
}


BaseContext::BaseContext() {}

Context_impl::Context_impl(void* handle): _handle(handle) {}

Context_impl::~Context_impl() {}

MonoContext_impl::MonoContext_impl(void* handle): Context_impl(handle) {
  make_current();
}


MonoContext::MonoContext(void* handle): BaseContext() {
  _impl = new MonoContext_impl(handle);
}


void MonoContext_impl::make_current() {
  if (current_context && current_context != this) {
    current_context->on_made_not_current();
  }
  current_context = this;
}

bool MonoContext_impl::current() const {
  return current_context == this;
}


MonoContext_impl::~MonoContext_impl() {
  if (current_context == this) {
    current_context = nullptr;
  }
}


MultiContext_impl::MultiContext_impl(void* handle)
  : Context_impl(handle)
  , _thread_id(std::this_thread::get_id())
{
  make_current();
}

void MultiContext_impl::make_current() {
  if (std::this_thread::get_id() != _thread_id) {
    throw gl::exception("attempt to make Context current on another thread");
  }
  {
    std::lock_guard<std::recursive_mutex> lock(current_context_lock);
    auto& i = current_context[_thread_id]; 
    if (i != this) {
      if (i) {
        i->on_made_not_current();
      }
      i = this;
    }
  }
  Context_impl::make_current();
}

bool MultiContext_impl::current() const {
  if (std::this_thread::get_id() != _thread_id) {
    return false;
  }
  std::lock_guard<std::recursive_mutex> lock(current_context_lock);
  auto it = current_context.find(_thread_id);
  return it != current_context.end() && it->second == this;
}

MultiContext_impl::~MultiContext_impl() {
  std::lock_guard<std::recursive_mutex> lock(current_context_lock);
  auto it = current_context.find(_thread_id);
  if (it != current_context.end() && it->second == this) {
    current_context.erase(it);
  }
}


bool BaseContext::current() const {
  return _impl->current();
}

void BaseContext::make_current() {
  _impl->make_current();
}


MonoContext::~MonoContext() {}





} // namespace gl
