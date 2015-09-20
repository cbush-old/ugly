#include "log.h"
#include "ugly.h"

#include "glfw_app.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <functional>
#include <tuple>

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




void test_enable_disable(gl::Context& context) {
  using enable_f = void(gl::Context::*)();
  using disable_f = void(gl::Context::*)();
  using is_enabled_f = bool(gl::Context::*)();
  using threeway = std::tuple<const char*, enable_f, disable_f, is_enabled_f>;

#define FTUPLE(CAP) \
  std::make_tuple<const char*, enable_f, disable_f, is_enabled_f>( \
    #CAP, &gl::Context::enable< CAP >, &gl::Context::disable< CAP >, &gl::Context::is_enabled< CAP > \
  )

  static const std::vector<threeway> funcs {
    FTUPLE(GL_BLEND),
    FTUPLE(GL_COLOR_LOGIC_OP),
    FTUPLE(GL_CULL_FACE),
    FTUPLE(GL_DEPTH_CLAMP),
    FTUPLE(GL_DEPTH_TEST),
    FTUPLE(GL_DITHER),
    FTUPLE(GL_FRAMEBUFFER_SRGB),
    FTUPLE(GL_LINE_SMOOTH),
    FTUPLE(GL_MULTISAMPLE),
    FTUPLE(GL_POLYGON_OFFSET_FILL),
    FTUPLE(GL_POLYGON_OFFSET_LINE),
    FTUPLE(GL_POLYGON_OFFSET_POINT),
    FTUPLE(GL_POLYGON_SMOOTH),
    FTUPLE(GL_PRIMITIVE_RESTART),
    FTUPLE(GL_RASTERIZER_DISCARD),
    FTUPLE(GL_SAMPLE_ALPHA_TO_COVERAGE),
    FTUPLE(GL_SAMPLE_ALPHA_TO_ONE),
    FTUPLE(GL_SAMPLE_COVERAGE),
    FTUPLE(GL_SAMPLE_SHADING),
    FTUPLE(GL_SAMPLE_MASK),
    FTUPLE(GL_SCISSOR_TEST),
    FTUPLE(GL_STENCIL_TEST),
    FTUPLE(GL_TEXTURE_CUBE_MAP_SEAMLESS),
    FTUPLE(GL_PROGRAM_POINT_SIZE),
  };

  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto enable = std::get<1>(t);
    auto disable = std::get<2>(t);
    auto is_enabled = std::get<3>(t);
    bool enabled = (context.*is_enabled)();
    logi("%s %s enabled", name, enabled ? "is" : "is not");

    for (int i = 0; i < 2; ++i) {
      if (enabled) {
        (context.*disable)();
        enabled = (context.*is_enabled)();
        expect("after 'disable', disabled", !enabled);
      } else {
        (context.*enable)();
        enabled = (context.*is_enabled)();
        expect("after 'enable', enabled", enabled);
        (context.*enable)();
      }
    }
  }




}








int main(int argc, const char* const argv[]) {
  try {

  glfwApp app;

  //  CONTEXT TESTS
  //
  //
  gl::MonoContext context1(&app);
  expect("after creating context1, context1 is current", context1.current());

  gl::MonoContext context2(&app);
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
  gl::Program program1 (vert, frag);
  program1.use(context1);


  gl::Program program2 (
    context1,
    gl::VertexShader("test/shaders/vert.glsl"),
    gl::FragmentShader("test/shaders/frag.glsl")
  );

  {
    gl::uniform<int> a (program2, "blah");
    expect("non-existent uniform not found", a.location(), -1);
  }

  {
    gl::uniform<int> a (program2, "color");
    expect("existent uniform found", a.location() != -1);

    gl::uniform<int> b = a;
    expect("uniform copy succeeded", a.location() == b.location());
    expect("uniform equality works", a == b);
  }

  gl::uniform4<float> color (program2, "color");

  struct {
    int x, y, z, w;
  } vec;
  color.set(vec);

  gl::Texture tex0;


  test_enable_disable(context1);

  gl::color c (0.f, 0.f, 0.f, 1.f);
  float i = 0;
  while (!app.done()) {
    context1.clear();
    i += 0.01;
    c.r = 0.5 + std::sin(i) * 0.5;
    c.g = 0.5 + std::sin(i + M_PI_2) * 0.5;
    c.b = 0.5 + std::sin(i + M_PI_2 * 2.f) * 0.5;
    context1.clear_color(c);
    app.update();
  }

  } catch(gl::exception const& e) {
    loge("caught exception: %s", e.what()); 
  }
}
