#include "log.h"
#include "ugly.h"

#include "glfw_app.h"

#include <thread>
#include <chrono>

int main(int argc, const char* const argv[]) {

  glfwApp app;
  gl::Context context(&app);
  gl::Program program;
  gl::VertexShader vert ("test/shaders/vert.glsl");
  gl::FragmentShader frag ("test/shaders/frag.glsl");
  program.attach(vert);
  program.attach(frag);
  program.link();
  program.use(context);


  while (!app.done()) {
    app.update();
  }

}
