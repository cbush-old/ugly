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
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get<unsigned, NAME>)
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
    F(GL_MAX_VERTEX_ATTRIBS),
    F(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS),
    F(GL_MAX_VERTEX_UNIFORM_COMPONENTS),
    F(GL_MAX_VERTEX_UNIFORM_VECTORS),
    F(GL_MAX_VERTEX_OUTPUT_COMPONENTS),
    F(GL_MAX_VERTEX_UNIFORM_BLOCKS),
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
    F(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT),
    F(GL_UNPACK_ALIGNMENT),
    F(GL_UNPACK_IMAGE_HEIGHT),
    F(GL_UNPACK_ROW_LENGTH),
    F(GL_UNPACK_SKIP_IMAGES),
    F(GL_UNPACK_SKIP_PIXELS),
    F(GL_UNPACK_SKIP_ROWS),
    F(GL_VIEWPORT_SUBPIXEL_BITS),
    //F(GL_MAX_VARYING_FLOATS),
    //F(GL_MAX_VIEWPORT_DIMS),
    //F(GL_TRANSFORM_FEEDBACK_BUFFER_START),
    //F(GL_UNIFORM_BUFFER_START),
  };
#undef F
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %u", name, value);
  }

}


void test_float_gets(gl::Context const& context) {
  using getf = float(gl::Context::*)() const;
  using name_and_func = std::tuple<const char*, getf>;
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get<float, NAME>)
  static const std::vector<name_and_func> funcs {
    F(GL_LINE_WIDTH),
    F(GL_MAX_TEXTURE_LOD_BIAS),
    F(GL_POLYGON_OFFSET_FACTOR),
    F(GL_POLYGON_OFFSET_UNITS),
    F(GL_SAMPLE_COVERAGE_VALUE),
    F(GL_STENCIL_BACK_REF),
    F(GL_STENCIL_REF),
    F(GL_SMOOTH_LINE_WIDTH_GRANULARITY),
  };
#undef F
  logi("test float gets");
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %f", name, value);
  }

}


void test_range_gets(gl::Context const& context) {
  using getf = gl::range(gl::Context::*)() const;
  using name_and_func = std::tuple<const char*, getf>;
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get<gl::range, NAME>)
  static const std::vector<name_and_func> funcs {
    F(GL_DEPTH_RANGE),
    F(GL_POINT_SIZE_RANGE),
    F(GL_SMOOTH_LINE_WIDTH_RANGE),
    F(GL_ALIASED_LINE_WIDTH_RANGE),
  };
#undef F
  logi("test range gets");
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %f - %f", name, value.low, value.high);
  }

}


void test_int_gets(gl::Context const& context) {
  using getf = int(gl::Context::*)() const;
  using name_and_func = std::tuple<const char*, getf>;
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get<int, NAME>)
  static const std::vector<name_and_func> funcs {
    F(GL_MAJOR_VERSION),
    F(GL_MAX_PROGRAM_TEXEL_OFFSET),
    F(GL_MIN_PROGRAM_TEXEL_OFFSET),
    F(GL_SAMPLE_BUFFERS),
    F(GL_SAMPLES),
    //F(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE),
    //F(GL_UNIFORM_BUFFER_SIZE),
    F(GL_DEPTH_CLEAR_VALUE),
  };
#undef F
  logi("test int gets");
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %d", name, value);
  }

}


void test_enum_gets(gl::Context const& context) {
  using getf = GLenum(gl::Context::*)() const;
  using name_and_func = std::tuple<const char*, getf>;
#define F(NAME) std::make_tuple<const char*, getf>(#NAME, &gl::Context::get<GLenum, NAME>)
  static const std::vector<name_and_func> funcs {
    F(GL_BLEND_DST_ALPHA),
    F(GL_BLEND_DST_RGB),
    F(GL_BLEND_EQUATION_RGB),
    F(GL_BLEND_EQUATION_ALPHA),
    F(GL_BLEND_SRC_ALPHA),
    F(GL_BLEND_SRC_RGB),
    F(GL_DEPTH_FUNC),
    F(GL_DRAW_BUFFER),
    F(GL_FRAGMENT_SHADER_DERIVATIVE_HINT),
    F(GL_IMPLEMENTATION_COLOR_READ_FORMAT),
    F(GL_IMPLEMENTATION_COLOR_READ_TYPE),
    F(GL_LINE_SMOOTH_HINT),
    F(GL_LAYER_PROVOKING_VERTEX),
    F(GL_LOGIC_OP_MODE),
    F(GL_PIXEL_PACK_BUFFER_BINDING),
    F(GL_PIXEL_UNPACK_BUFFER_BINDING),
    F(GL_PROVOKING_VERTEX),
    F(GL_POLYGON_SMOOTH_HINT),
    F(GL_READ_BUFFER),
    F(GL_STENCIL_BACK_FAIL),
    F(GL_STENCIL_BACK_FUNC),
    F(GL_STENCIL_BACK_PASS_DEPTH_FAIL),
    F(GL_STENCIL_BACK_PASS_DEPTH_PASS),
    F(GL_STENCIL_FAIL),
    F(GL_STENCIL_FUNC),
    F(GL_STENCIL_PASS_DEPTH_FAIL),
    F(GL_STENCIL_PASS_DEPTH_PASS),
    F(GL_TEXTURE_COMPRESSION_HINT),
    F(GL_VIEWPORT_INDEX_PROVOKING_VERTEX),
  };
#undef F
  logi("test enum gets");
  for (auto const& t : funcs) {
    auto name = std::get<0>(t);
    auto f = std::get<1>(t);
    auto value = (context.*f)();
    logi("%s = %x (%s)", name, value, gl::to_string(value));
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
  test_float_gets(context1);
  test_range_gets(context1);
  test_int_gets(context1);
  test_enum_gets(context1);





  auto active_texture = context1.active_texture();
  expect("initial active texture == 0", active_texture, 0);
  context1.active_texture(55);
  active_texture = context1.active_texture();
  expect("after setting active texture unit, active texture unit == 55", active_texture, 55);




  gl::color c (0.f, 0.f, 0.f, 1.f);
  context1.clear_color(c);
  gl::color curr = context1.get<gl::color, GL_COLOR_CLEAR_VALUE>();
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
