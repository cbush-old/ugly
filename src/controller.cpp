#include "controller.h"
#include <set>

namespace gl {

class Controller_impl {
  public:
    std::set<BaseContext*> contexts;
};


Controller::Controller(): _impl(new Controller_impl) {}

Controller::Controller(BaseContext& context): Controller() {
  context.attach(*this);
  _impl->contexts.insert(&context);
}

Controller::~Controller() {
  while (_impl->contexts.size()) {
    BaseContext *context = *_impl->contexts.begin();
    context->detach(*this);
  }
  delete _impl;
}

/*
void Controller::on_attached(BaseContext& context) {
  _impl->contexts.insert(&context);
}

void Controller::on_detached(BaseContext& context) {
  _impl->contexts.erase(&context);
}
*/

}

