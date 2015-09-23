#include "glfw_app.h"
#include "log.h"

bool glfwApp::_done { true };
unsigned glfwApp::_refs { 0 };

void glfwApp::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  _done = true;
}

void glfwApp::on_error(int err, const char* message) {
  loge("glfw error %d: %s", err, message);
  throw 1;
}

glfwApp::glfwApp(int major, int minor) {
  if (!_refs) {
    glfwSetErrorCallback(on_error);
    glfwInit();
  }
  ++_refs;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

  _window = glfwCreateWindow(640, 480, "libugly test", NULL, NULL);
  _done = false;

  glfwSetKeyCallback(_window, key_callback);
}

glfwApp::~glfwApp() {
  glfwDestroyWindow(_window);
  --_refs;
  if (!_refs) {
    glfwTerminate();
  }
}

void glfwApp::update() {
  glfwPollEvents();
  glfwSwapBuffers(_window);
}

void glfwApp::make_current() {
  glfwMakeContextCurrent(_window);
}

bool glfwApp::done() const {
  return _done;
}
