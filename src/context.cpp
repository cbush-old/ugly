#include "context.h"

namespace gl {

MonoContext* MonoContext::current_context { 0 };

std::map<std::thread::id, MultiContext*> MultiContext::current_context;
std::recursive_mutex MultiContext::current_context_lock;

// Lookup table for BufferIndex to GL_TARGET_BUFFER enums.
// Obviously, BufferIndex order must match this table order.
// Have you updated either table recently? Do the orders 
// still match? Well, do they? Did you double check...?!
const GLenum BaseContext::buffer_target[BUFFER_INDEX_MAX] {
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


ContextAssociatedObject::ContextAssociatedObject(IContext& context)
  : _context(context)
{
  _context.add(this);
}

ContextAssociatedObject::~ContextAssociatedObject() {
  _context.remove(this);
}






void BaseContext::add(ContextAssociatedObject* object) {
  _associated_objects.insert(object);
}

void BaseContext::remove(ContextAssociatedObject* object) {
  _associated_objects.erase(object);
}

BaseContext::BaseContext(void* handle)
  : _handle(handle) 
  {}

BaseContext::~BaseContext() {}

void BaseContext::attach(IController& controller) {
  controller.activate(*this);
  _attached_controllers.insert(&controller);
}

void BaseContext::detach(IController& controller) {
  _attached_controllers.erase(&controller);
  controller.deactivate(*this);
}


void BaseContext::on_made_not_current() {
  for (IController* controller : _attached_controllers) {
    controller->deactivate(*this);
  }
  _attached_controllers.clear();
}


MonoContext::MonoContext(void* handle): BaseContext(handle) {}


void MonoContext::make_current() {
  if (current_context && current_context != this) {
    current_context->on_made_not_current();
  }
  current_context = this;
}

bool MonoContext::current() const {
  return current_context == this;
}


MonoContext::~MonoContext() {
  if (current_context == this) {
    current_context = nullptr;
  }
}


MultiContext::MultiContext(void* handle)
  : BaseContext(handle)
  , _thread_id(std::this_thread::get_id())
  {}

void MultiContext::make_current() {
  if (std::this_thread::get_id() != _thread_id) {
    throw;
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
  BaseContext::make_current();
}

bool MultiContext::current() const {
  if (std::this_thread::get_id() != _thread_id) {
    throw;
  }
  std::lock_guard<std::recursive_mutex> lock(current_context_lock);
  auto it = current_context.find(_thread_id);
  return it != current_context.end() && it->second == this;
}

MultiContext::~MultiContext() {
  std::lock_guard<std::recursive_mutex> lock(current_context_lock);
  auto it = current_context.find(_thread_id);
  if (it != current_context.end() && it->second == this) {
    current_context.erase(it);
  }
}


} // namespace gl
