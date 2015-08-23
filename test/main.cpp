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
  try {

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

  expect("gl major version is 4", context1.major_version(), 4u);
  expect("gl minor version is 1", context1.minor_version(), 1u);


  //  SHADER TESTS
  //
  //
  {
    gl::VertexShader vert ("test/shaders/vert.glsl");
    expect("shader compiled", vert.compiled());

    EXPECT_THROW("bad file path threw", gl::VertexShader bad_path("garbage"))
    EXPECT_THROW("compiling shader with bad syntax failed", gl::VertexShader syntax_error_vert("test/shaders/bad_syntax.glsl"));

    gl::VertexShader shader;
    shader.set_source("hello");
    expect("after setting single shader source, source matches input", shader.get_source(), "hello\n");

    shader.set_source({
      "hello", "cruel", "world",
    });
    expect("after setting multiple shader source, source matches input", shader.get_source(), "hellocruelworld\n");
  }

  gl::VertexShader vert ("test/shaders/vert.glsl");
  gl::FragmentShader frag ("test/shaders/frag.glsl");
  gl::Program program (vert, frag);
  program.use(context1);


  gl::Program program2 (
    context1,
    gl::VertexShader("test/shaders/vert.glsl"),
    gl::FragmentShader("test/shaders/frag.glsl")
  );

  gl::uniform<int> a = program2.get_uniform<int>("blah");
  gl::uniform<int> b;
  expect("uniform copy matches", a.location(), b.location());

  while (!app.done()) {
    context1.clear();
    app.update();
  }

  } catch(gl::exception const& e) {
    loge("caught exception: %s", e.what()); 
  }
}
