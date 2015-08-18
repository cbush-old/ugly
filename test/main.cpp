#include "ogl.h"
#include "log.h"

#include "pipeline.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>

bool done { false };

void on_glfw_error(int err, const char* message) {
  loge("glfw error %d: %s", err, message);
  throw 1;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  done = true;
}


int main(int argc, const char* const argv[]) {
  glfwSetErrorCallback(on_glfw_error);
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

  GLFWwindow* window = glfwCreateWindow(640, 480, "ogl test", NULL, NULL);

  glfwSetKeyCallback(window, key_callback);

  while (!done) {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

}
