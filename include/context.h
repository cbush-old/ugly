#ifndef CONTEXT_H
#define CONTEXT_H

#include "gl_type.h"


namespace gl {

class Controller;

class BaseContext {
  public:
    BaseContext();

  public:
    virtual ~BaseContext() =0;
    BaseContext(BaseContext const&) = delete;
    BaseContext& operator=(BaseContext const&) = delete;

  public: // OPENGL VERSION INFO
    /**
     * @brief retrieve the major OpenGL version
     **/
    unsigned major_version() const;

    /**
     * @brief retrieve the minor OpenGL version
     **/
    unsigned minor_version() const;

  public:
    /**
     * @brief make this Context the current Context of this thread.
     **/
    void make_current();

    /**
     * @brief check whether the Context is current on this thread.
     * @return true if the Context is current on this thread.
     **/
    bool current() const;

  public:
    void clear();
    void clear(GLbitfield mask);

  public:
    void attach(Controller& controller);
    void detach(Controller& controller);

  protected:
    class Context_impl* _impl { nullptr };

};

/**
 * @brief a Context to be used when Context activity will always happen on the same thread.
 **/
class MonoContext : public BaseContext {
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
class MultiContext : public BaseContext {
  public:
    MultiContext(void*);
    ~MultiContext() override;
};


using Context = MonoContext;

}


#endif
