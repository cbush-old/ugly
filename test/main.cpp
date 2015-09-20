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
  using is_enabled_f = bool(gl::Context::*)() const;
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

#undef FTUPLE

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



void test_unsigned_gets(gl::Context const& context) {
  using getf = unsigned(gl::Context::*)() const;
  using name_and_func = std::tuple<const char*, getf>;
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get_unsigned<NAME>)
  static const std::vector<name_and_func> funcs {
    F(GL_CONTEXT_FLAGS),
    F(GL_MAX_3D_TEXTURE_SIZE),
    F(GL_MAX_ARRAY_TEXTURE_LAYERS),
    F(GL_MAX_CLIP_DISTANCES),
    F(GL_MAX_COLOR_TEXTURE_SAMPLES),
    F(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS),
    F(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS),
    F(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS),
    F(GL_MAX_COMBINED_UNIFORM_BLOCKS),
    F(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS),
    F(GL_MAX_CUBE_MAP_TEXTURE_SIZE),
    F(GL_MAX_DEPTH_TEXTURE_SAMPLES),
    F(GL_MAX_DRAW_BUFFERS),
    F(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS),
    F(GL_MAX_ELEMENTS_INDICES),
    F(GL_MAX_ELEMENTS_VERTICES),
    F(GL_MAX_FRAGMENT_INPUT_COMPONENTS),
    F(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS),
    F(GL_MAX_FRAGMENT_UNIFORM_VECTORS),
    F(GL_MAX_FRAGMENT_UNIFORM_BLOCKS),
    F(GL_MAX_GEOMETRY_INPUT_COMPONENTS),
    F(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS),
    F(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS),
    F(GL_MAX_GEOMETRY_UNIFORM_BLOCKS),
    F(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS),
    F(GL_MAX_INTEGER_SAMPLES),
    F(GL_MAX_RECTANGLE_TEXTURE_SIZE),
    F(GL_MAX_RENDERBUFFER_SIZE),
    F(GL_MAX_SAMPLE_MASK_WORDS),
    F(GL_MAX_SERVER_WAIT_TIMEOUT),
    F(GL_MAX_TEXTURE_BUFFER_SIZE),
    F(GL_MAX_TEXTURE_IMAGE_UNITS),
    F(GL_MAX_TEXTURE_SIZE),
    F(GL_MAX_UNIFORM_BUFFER_BINDINGS),
    F(GL_MAX_UNIFORM_BLOCK_SIZE),
    F(GL_MAX_VARYING_VECTORS),
    F(GL_MAX_VARYING_FLOATS),
    F(GL_MAX_VERTEX_ATTRIBS),
    F(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS),
    F(GL_MAX_VERTEX_UNIFORM_COMPONENTS),
    F(GL_MAX_VERTEX_UNIFORM_VECTORS),
    F(GL_MAX_VERTEX_OUTPUT_COMPONENTS),
    F(GL_MAX_VERTEX_UNIFORM_BLOCKS),
    F(GL_MAX_VIEWPORT_DIMS),
    F(GL_MAX_VIEWPORTS),
    F(GL_MINOR_VERSION),
    F(GL_NUM_COMPRESSED_TEXTURE_FORMATS),
    F(GL_NUM_EXTENSIONS),
    F(GL_NUM_PROGRAM_BINARY_FORMATS),
    F(GL_NUM_SHADER_BINARY_FORMATS),
    F(GL_PACK_ALIGNMENT),
    F(GL_PACK_IMAGE_HEIGHT),
    F(GL_PACK_ROW_LENGTH),
    F(GL_PACK_SKIP_IMAGES),
    F(GL_PACK_SKIP_PIXELS),
    F(GL_PACK_SKIP_ROWS),
    F(GL_POINT_FADE_THRESHOLD_SIZE),
    F(GL_PRIMITIVE_RESTART_INDEX),
    F(GL_POINT_SIZE),
    F(GL_POINT_SIZE_GRANULARITY),
    F(GL_STENCIL_BACK_VALUE_MASK),
    F(GL_STENCIL_BACK_WRITEMASK),
    F(GL_STENCIL_CLEAR_VALUE),
    F(GL_STENCIL_VALUE_MASK),
    F(GL_STENCIL_WRITEMASK),
    F(GL_SUBPIXEL_BITS),
    F(GL_TRANSFORM_FEEDBACK_BUFFER_START),
    F(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT),
    F(GL_UNIFORM_BUFFER_START),
    F(GL_UNPACK_ALIGNMENT),
    F(GL_UNPACK_IMAGE_HEIGHT),
    F(GL_UNPACK_ROW_LENGTH),
    F(GL_UNPACK_SKIP_IMAGES),
    F(GL_UNPACK_SKIP_PIXELS),
    F(GL_UNPACK_SKIP_ROWS),
    F(GL_VIEWPORT_SUBPIXEL_BITS),
  };
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %u", name, value);
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
  test_unsigned_gets(context1);
  expect("blend enabled", context1.get_bool<GL_BLEND>());

  context1.get_bool<GL_BLEND>();

  gl::color c (0.f, 0.f, 0.f, 1.f);
  context1.clear_color(c);
  gl::color curr = context1.get_color<GL_COLOR_CLEAR_VALUE>();
  expect("GL_COLOR_CLEAR_VALUE == set color", curr == c);
  float i = 0;
  while (!app.done()) {
    context1.clear();
    i += 0.01f;
    c.r = 0.5 + std::sin(i) * 0.5;
    c.g = 0.5 + std::sin(i + M_PI_2) * 0.5;
    c.g = 0.5 + std::sin(i + M_PI_2 * 2.f) * 0.5;
    context1.clear_color(c);
    app.update();
  }

  } catch(gl::exception const& e) {
    loge("caught exception: %s", e.what()); 
  }
}
