#include "context.h"

namespace gl {

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

MonoContext::MonoContext(void* handle): BaseContext(handle) {}

void MonoContext::make_current() {
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
  std::lock_guard<std::recursive_mutex> lock(current_context_lock);

}

bool MultiContext::current() const {
  return false;
}

MultiContext::~MultiContext() {

}


} // namespace gl
