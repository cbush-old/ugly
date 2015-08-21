#include "controller.h"

namespace gl {

void Controller::activate(IContext& context) {
  _context = &context;
}

void Controller::deactivate(IContext&) {
  _context = nullptr;
}

bool Controller::active() const {
  return _context;
}

BlendController::~BlendController() {}

void BlendController::func(GLenum src, GLenum dst) {
  if (!active()) {
    throw gl::exception("blend controller is not active");
  }
  GL_CALL(glBlendFunc(src, dst));
}

void BlendController::func(GLuint buf, GLenum src, GLenum dst) {
  if (!active()) {
    throw gl::exception("blend controller is not active");
  }
  // validate buf, src, dst
  GL_CALL(glBlendFunci(buf, src, dst));
}


DepthController::~DepthController() {}


ViewportController::~ViewportController() {}




}

