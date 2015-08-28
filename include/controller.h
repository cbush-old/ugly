#ifndef UGLY_CONTROLLER_H
#define UGLY_CONTROLLER_H

#include "context.h"

namespace gl {

class Controller {
  public:
    Controller();
    Controller(BaseContext& context);
    virtual ~Controller();

  protected:
    bool active() const;

  private:
    class Controller_impl* _impl;

};

class ViewportController : public Controller {};
class CullfaceController : public Controller {};
class DepthController : public Controller {};
class ColorController : public Controller {};

} // namespace gl


#endif
