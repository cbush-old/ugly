#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"

#include <set>
#include <map>
#include <thread>
#include <mutex>

namespace gl {


class BaseContext : public IContext {
  public:
    BaseContext(void*);
    ~BaseContext();

  public:
    void set_viewport(IViewport&) override;
    void set_blend(IBlend&) override;
    void set_cullface(ICullface&) override;
    void set_depth(IDepth&) override;
    void set_color(IColor&) override;

  public: // OPENGL VERSION INFO
    int get_major_version() const;
    int get_minor_version() const;
    template<typename T>
    T get(GLenum) const;

  public:
    /**
     * @brief Bind a Program to the Context.
     **/
    virtual void bind(IProgram const&) =0;

    /**
     * @brief Unbind a Program from the Context.
     **/
    virtual void unbind(IProgram const&) =0;


  protected:
    void add(ContextAssociatedObject*) override;
    void remove(ContextAssociatedObject*) override;

  private:
    void *_handle { nullptr };
    bool _current { true };

    std::set<ContextAssociatedObject const*> _associated_objects;

};

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
