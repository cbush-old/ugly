#include "controller.h"

namespace gl {

void Controller::activate(IContext& context) {
  _context = &context;
}

void Controller::deactivate() {
  _context = nullptr;
}

bool Controller::is_active() const {
  return _context && _context->current();
}

void Controller::commit() {
  if (!is_active()) {
    throw;
  }

  for (auto& f : _queue) {
    f();
  }
  _queue.clear();
}


BlendController::~BlendController() {}

void BlendController::func(GLenum src, GLenum dst) {
  // validate src, dst
  post([=] {
    GL_CALL(glBlendFunc(src, dst));
  });
}

void BlendController::func(GLuint buf, GLenum src, GLenum dst) {
  // validate buf, src, dst
  post([=] {
    GL_CALL(glBlendFunci(buf, src, dst));
  });
}

void BlendController::func_separate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  post([=] {
    GL_CALL(glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha));
  });
}

void BlendController::func_separate(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
  post([=] {
    GL_CALL(glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha));
  });
}


DepthController::~DepthController() {}


ViewportController::~ViewportController() {}




}

