#include "log.h"
#include "ugly.h"

#include "glfw_app.h"

#include <iostream>
#include <thread>
#include <chrono>


template<typename T, typename U>
void expect(const char* name, T result, U expected) {
  if (!(result == expected)) {
    std::cerr << "\033[41m FAILED: \033[0m " << name
      << ": got " << result << ", expected " << expected << std::endl;
  } else {
    std::cerr << "\033[42m SUCCESS: \033[0m " << name << std::endl;
  }
}

void expect(const char* name, bool result) {
  expect(name, result, true);
}

#define EXPECT_THROW(name, stmt) {\
  bool threw = false; \
  try { stmt; } catch(...) { threw = true; } \
  expect(name, threw); }


int main(int argc, const char* const argv[]) {

  glfwApp app;

  //  CONTEXT TESTS
  //
  //
  gl::Context context1(&app);
  expect("after creating context1, context1 is current", context1.current());

  gl::Context context2(&app);
  expect("after creating context2, context2 is current", context2.current());
  expect("after creating context2, context1 is not current", !context1.current());

  context1.make_current();
  expect("after making context1 current, context2 is not current", !context2.current());
  expect("after making context1 current, context1 is current", context1.current());


  //  SHADER TESTS
  //
  //
  gl::VertexShader vert ("test/shaders/vert.glsl");
  expect("shader compiled", vert.compiled());

  EXPECT_THROW("bad file path threw", gl::VertexShader bad_path("garbage"))
  EXPECT_THROW("compiling shader with bad syntax failed", gl::VertexShader syntax_error_vert("test/shaders/bad_syntax.glsl"));

  gl::FragmentShader frag ("test/shaders/frag.glsl");


  gl::Program program;
  program.attach(vert);
  program.attach(frag);
  program.link();
  program.use(context1);


  while (!app.done()) {
    app.update();
  }

}
