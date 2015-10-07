#include "log.h"
#include "ugly.h"

#include "glfw_app.h"


int main(int argc, const char* const argv[]) {

  glfwApp app;

  gl::MonoContext context (&app);

  gl::Program program (
    context,
    gl::VertexShader("test/shaders/vert.glsl"),
    gl::FragmentShader("test/shaders/frag.glsl")
  );

  std::cout << program.uniform_location("color") << std::endl;

  gl::uniform4<float> color (program, program.uniform_location("color"));

  std::cout << color.location() << std::endl;
  //std::cout << color.get() << std::endl;


  while (!app.done()) {
    context.clear();
    app.update();
  }

}
