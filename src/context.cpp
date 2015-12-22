#include "context.h"

#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

#include "buffer.h"
#include "program.h"
#include "texture.h"
#include "vertex_array.h"


namespace gl {




// My policy with the impl so far is only to forward function calls where I actually
// need polymorphism. Mostly, I'll use impl class as an opaque data storage type.
// And I'm using pimpl at all, rather than pure virtual interface inheritance,
// because:
//   -  I want users to be able to put the concrete types on their stacks
//   -  I don't want to write factories all over the place
//   -  There's no intention to have multiple implementations of most things
//       (I mean, it's a wrapper...)
//   -  I'm trying to avoid future ABI compatibility issues
//   -  It's faster  :trollface:

class Context_impl {
  public:
    Context_impl(Context& context, void * handle);
    virtual ~Context_impl() =0;


  public:
    virtual void make_current() {}
    virtual bool current() const { return false; }
    void on_made_not_current();

  public:
    GLbitfield _clear_mask { GL_COLOR_BUFFER_BIT };

  protected:
    Context& _context;
    void *_handle { nullptr };
};


class MonoContext_impl : public Context_impl {
  private:
    static MonoContext_impl* current_context;

  public:
    MonoContext_impl(Context& context, void *);
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
    MultiContext_impl(MultiContext& context, void *);
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
inline void get(Context const& context, GLenum param, T* out) {
  if (!context.current()) {
    throw gl::exception("can't get param from inactive context");
  }
  GL_CALL(GET(param, out));
}

template<typename T, void(*GET)(GLenum, T*)>
inline T get(Context const& context, GLenum param) { // get single
  T rv;
  get<T, GET>(context, param, &rv);
  return rv;
}

template<>
int Context::get<int>(GLenum param) const {
  return gl::get<GLint, glGetIntegerv>(*this, param);
}

template<>
unsigned Context::get<unsigned>(GLenum param) const {
  return gl::get<GLint, glGetIntegerv>(*this, param);
}

template<>
inline int64_t Context::get<int64_t>(GLenum param) const {
  return gl::get<GLint64, glGetInteger64v>(*this, param);
}

template<>
bool Context::get<bool>(GLenum param) const {
  return gl::get<GLboolean, glGetBooleanv>(*this, param);
}

template<>
double Context::get<double>(GLenum param) const {
  return gl::get<GLdouble, glGetDoublev>(*this, param);
}

template<>
float Context::get<float>(GLenum param) const {
  return gl::get<GLfloat, glGetFloatv>(*this, param);
}

template<>
color Context::get<color>(GLenum param) const {
  color c;
  gl::get<GLfloat, glGetFloatv>(*this, param, reinterpret_cast<GLfloat*>(&c));
  return c;
}

template<>
range Context::get<range>(GLenum param) const {
  range r;
  gl::get<GLfloat, glGetFloatv>(*this, param, reinterpret_cast<GLfloat*>(&r));
  return r;
}

inline std::vector<int> get(Context const& context, GLenum param, size_t size) {
  std::vector<int> v (size);
  gl::get<GLint, glGetIntegerv>(context, param, v.data());
  return v;
}


unsigned Context::major_version() const {
  return get<unsigned>(GL_MAJOR_VERSION);
}

unsigned Context::minor_version() const {
  return get<unsigned>(GL_MINOR_VERSION);
}

Context::~Context() {
  delete _impl;
}


void Context_impl::on_made_not_current() {
}


void Context::clear(GLenum mask) {
  GL_CALL(glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a));
  GL_CALL(glClear(mask));
}

void Context::draw(VertexArray const& vao, GLenum mode, GLsizei count, GLsizei first /* = 0 */) {
  VertexArrayBindguard guard(vao);
  ProgramBindguard program_guard(*vao.program());
  GL_CALL(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
  GL_CALL(glDrawArrays(mode, first, count));
}


Context::Context() {}

Context_impl::Context_impl(Context& context, void* handle): _context(context), _handle(handle) {}

Context_impl::~Context_impl() {
}

MonoContext_impl::MonoContext_impl(Context& context, void* handle): Context_impl(context, handle) {
  make_current();
}


MonoContext::MonoContext(void* handle): Context() {
  _impl = new MonoContext_impl(*this, handle);
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


MultiContext_impl::MultiContext_impl(MultiContext& context, void* handle)
  : Context_impl(context, handle)
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


bool Context::current() const {
  return _impl->current();
}

void Context::make_current() {
  _impl->make_current();
}


MonoContext::~MonoContext() {}



template<GLenum capability>
void Context::enable() {
  GL_CALL(glEnable(capability));
}

template<GLenum capability>
void Context::disable() {
  GL_CALL(glDisable(capability));
}

template<GLenum capability>
bool Context::is_enabled() const {
  auto rv = GL_CALL(glIsEnabled(capability));
  return rv;
}

template<GLenum param, GLenum size_key>
std::vector<int> Context::get() const {
  size_t size = get<unsigned>(size_key);
  return gl::get(*this, param, size);
}




#define INTERN
#include "context_instantiations.inl"
#undef INTERN


} // namespace gl
