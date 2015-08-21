#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"

#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

namespace gl {

class BaseContext : public IContext {
  private:
    static const GLenum buffer_target[BUFFER_INDEX_MAX];

  public:
    BaseContext(void*);
    ~BaseContext();

  public:
    void attach(IController &) override;
    void detach(IController &) override;

  public: // OPENGL VERSION INFO
    int get_major_version() const;
    int get_minor_version() const;
    template<typename T>
    T get(GLenum) const;

  public:
    virtual void use(IProgram const&) =0;
    virtual void bind(IBuffer const& buffer, BufferIndex target) override;
    virtual void unbind(BufferIndex) override;

  protected:
    void add(ContextAssociatedObject*) override;
    void remove(ContextAssociatedObject*) override;

  private:
    void *_handle { nullptr };

    IProgram const* _program { nullptr };
    IBuffer const* _buffer[BUFFER_INDEX_MAX] { nullptr };

    std::set<ContextAssociatedObject const*> _associated_objects;

};


/**
 * @brief a Context to be used when Context activity will always happen on the same thread.
 **/
class MonoContext : public BaseContext {
  private:
    static MonoContext const* current_context;

  public:
    MonoContext(void*);
    ~MonoContext();

  public:
    void make_current() override;
    bool current() const override;

};


/**
 * @brief a Context to be used when Context activity might happen from multiple threads.
 *
 * Each MultiContext is tied to the thread on which it was created, and cannot be used
 * from any other thread. However, you can create another MultiContext on a different
 * thread.
 **/
class MultiContext : public BaseContext {
  private:
    static std::map<std::thread::id, MultiContext const*> current_context;
    static std::recursive_mutex current_context_lock;

  public:
    MultiContext(void*);
    ~MultiContext();

  public:
    void make_current() override;
    bool current() const override;

  private:
    std::thread::id _thread_id;

};


using Context = MonoContext;

}


#endif
