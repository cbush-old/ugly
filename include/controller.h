#ifndef UGLY_CONTROLLER_H
#define UGLY_CONTROLLER_H

#include "context.h"
#include <functional>
#include <vector>

namespace gl {

class Controller : public IContext::IController {
  public:
    virtual ~Controller() {}

  public:
    void activate(IContext&) override;
    void deactivate() override;

  public:
    void commit();

  protected:
    bool is_active() const;
    void post(std::function<void()>);

  private:
    IContext *_context { nullptr };
    std::vector<std::function<void()>> _queue;
};

class BlendController : public Controller {
  public:
    ~BlendController();

  public:
    void func(GLenum src, GLenum dst);
    void func(GLuint buf, GLenum src, GLenum dst);
    void func_separate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void func_separate(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

};

class ViewportController : public Controller {
  public:
    ~ViewportController();
};

class CullfaceController : public Controller {
  public:
    ~CullfaceController();
};

class DepthController : public Controller {
  public:
    ~DepthController();
};

class ColorController : public Controller {
  public:
    ~ColorController();
};


}


#endif
