#include "log.h"
#include "ugly.h"

#include "glfw_app.h"

int main(int argc, const char* const argv[]) {

  glfwApp app;

  gl::Context context (&app);

  gl::Program program (
    context,
    gl::VertexShader("test/shaders/vert.glsl"),
    gl::FragmentShader("test/shaders/frag.glsl")
  );

  

  while (!app.done()) {
    context.clear();
    app.update();
  }

}
