#ifndef UGLY_CONTROLLER_H
#define UGLY_CONTROLLER_H

#include "context.h"

namespace gl {

class Controller : public IContext::IController {
  public:
    virtual ~Controller() {}

  public:
    void activate(IContext&) override;
    void deactivate(IContext&) override;

  protected:
    bool active() const;

  private:
    IContext* _context { nullptr };
};

class BlendController : public Controller {
  public:
    ~BlendController();

  public:
    void func(GLenum src, GLenum dst);
    void func(GLuint buf, GLenum src, GLenum dst);

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
