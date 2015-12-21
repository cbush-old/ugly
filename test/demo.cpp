#include "log.h"
#include "ugly.h"

#include "glfw_app.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>


void set_vertex_data(gl::Buffer& buffer, gl::VertexArray& vao, gl::Program const& program) {

  float scale = 1.f;
  float t = 1.f;
  #define MAKE3D(x, y, z) x * scale * 0.5f, y * scale * 0.5f, z * scale * 0.5f

  std::vector<GLfloat> vertex_data {

    // Left face
    MAKE3D(+1, -1, -1), // LTB
    MAKE3D(+1, +1, -1), // LBB
    MAKE3D(+1, +1, +1), // LBF
    MAKE3D(+1, -1, +1), // LTF

    // Bottom Face
    MAKE3D(+1, +1, +1), // LBF
    MAKE3D(+1, +1, -1), // LBB
    MAKE3D(-1, +1, -1), // RBB
    MAKE3D(-1, +1, +1), // RBF

    // Right face
    MAKE3D(-1, -1, +1), // RTF
    MAKE3D(-1, +1, +1), // RBF
    MAKE3D(-1, +1, -1), // RBB
    MAKE3D(-1, -1, -1), // RTB

    // Top face
    MAKE3D(+1, -1, -1), // LTB
    MAKE3D(+1, -1, +1), // LTF
    MAKE3D(-1, -1, +1), // RTF
    MAKE3D(-1, -1, -1), // RTB

    // Back face
    MAKE3D(-1, -1, -1), // RTB
    MAKE3D(-1, +1, -1), // RBB
    MAKE3D(+1, +1, -1), // LBB
    MAKE3D(+1, -1, -1), // LTB

    // Front face
    MAKE3D(+1, -1, +1), // LTF
    MAKE3D(+1, +1, +1), // LBF
    MAKE3D(-1, +1, +1), // RBF
    MAKE3D(-1, -1, +1), // RTF

    0, 0, 0, t, t, t, t, 0, // Left
    0, 0, 0, t, t, t, t, 0, // Top
    0, 0, 0, t, t, t, t, 0, // Right
    0, 0, 0, t, t, t, t, 0, // Bottom
    0, 0, 0, t, t, t, t, 0, // Back
    0, 0, 0, t, t, t, t, 0, // Front

    // Left
    +0.000000f, -0.333333f, +0.000000f, // LTB
    -0.333333f, -0.333333f, +0.333333f, // LTF
    +0.000000f, +0.000000f, -0.333333f, // LBB
    -0.333333f, +0.000000f, +0.000000f, // LBF

    // Bottom
    -0.333333f, +0.000000f, +0.000000f, // LBF
    +0.000000f, +0.000000f, -0.333333f, // LBB
    +0.333333f, +0.333333f, -0.333333f, // RBB
    +0.000000f, +0.333333f, +0.000000f, // RBF

    // Right
    +0.000000f, +0.000000f, +0.333333f, // RTF
    +0.000000f, +0.333333f, +0.000000f, // RBF
    +0.333333f, +0.333333f, -0.333333f, // RBB
    +0.333333f, +0.000000f, +0.000000f, // RTB

    // Top
    +0.000000f, -0.333333f, +0.000000f, // LTB
    -0.333333f, -0.333333f, +0.333333f, // LTF
    +0.000000f, +0.000000f, +0.333333f, // RTF
    +0.333333f, +0.000000f, +0.000000f, // RTB

    // Back
    +0.333333f, +0.000000f, +0.000000f, // RTB
    +0.333333f, +0.333333f, -0.333333f, // RBB
    +0.000000f, +0.000000f, -0.333333f, // LBB
    +0.000000f, -0.333333f, +0.000000f, // LTB

    // Front
    -0.333333f, -0.333333f, +0.333333f, // LTF
    -0.333333f, +0.000000f, +0.000000f, // LBF
    +0.000000f, +0.333333f, +0.000000f, // RBF
    +0.000000f, +0.000000f, +0.333333f, // RTF

  };
  #undef MAKE3D

  buffer.data(vertex_data, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

  gl::attrib position (program, "position");
  gl::attrib texcoord (program, "texcoord_in");
  gl::attrib normal (program, "normal_in");

  GL_CALL(glPatchParameteri(GL_PATCH_VERTICES, 4));
  vao.pointer(buffer, position, 3, GL_FLOAT, GL_FALSE, 0, 0);
  vao.pointer(buffer, texcoord, 2, GL_FLOAT, GL_FALSE, 0, 24 * 3 * sizeof(GLfloat));
  vao.pointer(buffer, normal, 3, GL_FLOAT, GL_FALSE, 0, (24 * 3 + 8 * 6) * sizeof(GLfloat));

  vao.enable(position);
  vao.enable(texcoord);
  vao.enable(normal);

}



void set_boring_vertex_data(gl::Buffer& buffer, gl::VertexArray& vao, gl::Program const& program) {

  float scale = 1.f;
  float t = 1.f;
  #define MAKE3D(x, y, z) x * scale * 0.5f, y * scale * 0.5f, z * scale * 0.5f

  std::vector<GLfloat> vertex_data {
    // Front face
    MAKE3D(-1, -1, +1),
    MAKE3D(+1, -1, +1),
    MAKE3D(-1, +1, +1),
    MAKE3D(+1, +1, +1),

    // Right face
    MAKE3D(+1, +1, +1),
    MAKE3D(+1, -1, +1),
    MAKE3D(+1, +1, -1),
    MAKE3D(+1, -1, -1),

    // Back face
    MAKE3D(+1, -1, -1),
    MAKE3D(-1, -1, -1),
    MAKE3D(+1, +1, -1),
    MAKE3D(-1, +1, -1),

    // Left face
    MAKE3D(-1, +1, -1),
    MAKE3D(-1, -1, -1),
    MAKE3D(-1, +1, +1),
    MAKE3D(-1, -1, +1),

    // Bottom face
    MAKE3D(-1, -1, +1),
    MAKE3D(-1, -1, -1),
    MAKE3D(+1, -1, +1),
    MAKE3D(+1, -1, -1),

    // Move to top
    MAKE3D(+1, -1, -1),
    MAKE3D(-1, +1, +1),

    // Top Face
    MAKE3D(-1, +1, +1),
    MAKE3D(+1, +1, +1),
    MAKE3D(+1, +1, -1),
    MAKE3D(-1, +1, -1),

    0, 0, t, 0, 0, t, t, t, // Front
    0, t, 0, 0, t, t, t, 0, // Right
    0, 0, t, 0, 0, t, t, t, // Back
    0, t, 0, 0, t, t, t, 0, // Left
    0, t, 0, 0, t, t, t, 0, // Bottom
    t, 0, 0, 0, // Move to top
    0, 0, t, 0, 0, t, t, t // Top

  };
  #undef MAKE3D

  buffer.data(vertex_data, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

  gl::attrib position (program, "position");
  gl::attrib texcoord (program, "texcoord_in");

  vao.pointer(buffer, position, 3, GL_FLOAT, GL_FALSE, 0, 0);
  vao.pointer(buffer, texcoord, 2, GL_FLOAT, GL_FALSE, 0, 26 * 3 * sizeof(GLfloat));

  vao.enable(position);
  vao.enable(texcoord);

}


int main(int argc, const char* const argv[]) {

  try {

    glfwApp app;

    gl::MonoContext context (&app);
    context.viewport(0, 0, app.width(), app.height());

    context.disable<GL_CULL_FACE>();
    context.enable<GL_DEPTH_TEST>();


    gl::ProgramRef program (new gl::Program(
      gl::VertexShader("shaders/demo/vert.glsl"),
      gl::TessControlShader("shaders/demo/tess_control.glsl"),
      gl::TessEvaluationShader("shaders/demo/tess_eval.glsl"),
      gl::GeometryShader("shaders/demo/geo.glsl"),
      gl::FragmentShader("shaders/demo/frag.glsl")
    ));
    
    gl::ProgramRef boring_program (new gl::Program(
      gl::VertexShader("shaders/vert.glsl"),
      gl::FragmentShader("shaders/frag.glsl")
    ));

    gl::uniform4<float> ambient (program, "ambient");
    gl::uniform4<float> light_color (program, "light_color");
    gl::uniform3<float> light_direction (program, "light_direction");
    gl::uniform3<float> half_vector (program, "hv");
    gl::uniform<float> shininess (program, "shininess");
    gl::uniform<float> strength (program, "strength");
    gl::uniform_sampler sampler (program, "texture_unit");

    ambient.set(0.1f, 0.1f, 0.1f, 1.f);
    light_color.set(1.f, 1.f, 1.f, 1.f);
    shininess.set(100.f);
    strength.set(1.f);

    gl::uniform_mat4 modelview (program, "modelview");
    gl::uniform_mat4 normal_matrix (program, "normal_matrix");
    gl::uniform_mat4 projection (program, "projection");

    gl::uniform_mat4 boring_modelview (boring_program, "modelview");
    gl::uniform_mat4 boring_projection (boring_program, "projection");
    gl::uniform_sampler boring_sampler (boring_program, "texture_unit");

    glm::mat4 projection_matrix {
      glm::perspective<GLfloat>(
        45,
        float(app.width()) / float(app.height()),
        1.0,
        20.0
      )
    };

    projection.set(glm::value_ptr(projection_matrix));
    boring_projection.set(glm::value_ptr(projection_matrix));

    gl::VertexArray vao (program);
    gl::Buffer buffer;
    set_vertex_data(buffer, vao, *program);
    
    gl::VertexArray boring_vao (boring_program);
    {
      gl::Buffer boring_buffer;
      set_boring_vertex_data(boring_buffer, boring_vao, *boring_program);
    }

    // Set up texture
    //
    //
    int tw = 256;
    std::vector<uint32_t> pixels (tw * tw);
    for (size_t i = 0; i < pixels.size(); ++i) {
      pixels[i] = uint32_t((i / (double)(tw * tw)) * 0x7fffffff);
    }

    gl::TextureParams params {
      { GL_TEXTURE_MIN_FILTER, GL_LINEAR },
      { GL_TEXTURE_MAG_FILTER, GL_LINEAR },
      { GL_TEXTURE_WRAP_S, GL_REPEAT },
      { GL_TEXTURE_WRAP_T, GL_REPEAT },
    };


    gl::Texture2D texture (params);
    gl::ImageDesc2D desc (tw, tw, pixels.data());
    texture.image(0, desc);
  
    // Properties of the displayed object
    float angle = 0.f;
    glm::vec3 position (0.f, 0.f, -2.0f);
    float tick = 0.f;

    gl::TextureUnit unit (texture);


    gl::Texture2D texture2 (params);
    for (size_t i = 0; i < pixels.size(); ++i) {
      pixels[i] = 0xff0000ff;
    }
    texture2.image(0, desc);
    gl::TextureUnit unit2 (texture2);
    
    gl::Texture2D fb_texture (params);
    fb_texture.storage(1, tw, tw);

    gl::Framebuffer fb;
    fb.texture(GL_COLOR_ATTACHMENT0, fb_texture);

    gl::Renderbuffer rb (GL_DEPTH_COMPONENT24, tw, tw);
    fb.renderbuffer(GL_DEPTH_ATTACHMENT, rb);

    gl::TextureUnit fb_unit (fb_texture);

    fb.viewport(0, 0, tw, tw);

    logi("framebuffer status: %s", fb.status_str());

    while (!app.done()) {

      static float light_angle = 0.5f;
      glm::vec3 direction = glm::rotate(glm::vec3(0.f, 0.f, 1.f), light_angle, glm::vec3(0.f, 1.f, 0.f));
      light_direction.set(direction.x, direction.y, direction.z);

      glm::vec3 hv = glm::normalize(
        direction + glm::vec3(0.f, 0.f, 1.f)
      );
      half_vector.set(hv.x, hv.y, hv.z);


      tick += 0.01f;

      glm::mat4 modelview_matrix {
        glm::translate(
          glm::mat4(),
          position
        )
      };

      angle += 0.025f;
      glm::mat4 rotation;
      rotation = glm::rotate(rotation, angle, glm::vec3(0.5f, 0.5f, 0.f));
      rotation = glm::rotate(rotation, angle * 0.5f, glm::vec3(0.f, 0.f, 1.f));
      auto rotated_modelview_matrix = modelview_matrix * rotation;

      normal_matrix.set(glm::value_ptr(rotation));
      modelview.set(glm::value_ptr(rotated_modelview_matrix));

      fb.clear_color(
        0.5f + sin(tick) * 0.5f,
        0.5f + sin(tick + 1.f) * 0.5f,
        0.5f + sin(tick + 2.f) * 0.5f
      );

      fb.clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      sampler.use(unit);
      fb.draw(vao, GL_PATCHES, 24);
      context.clear_color(
        0.5f + cos(tick) * 0.5f,
        0.5f + cos(tick + 1.f) * 0.5f,
        0.5f + cos(tick + 2.f) * 0.5f
      );

      glm::mat4 rotation2;
      rotation2 = glm::rotate(rotation2, angle * 0.2f, glm::vec3(0.f, 1.f, 0.f));
      auto slow_rotated_modelview_matrix = modelview_matrix * rotation2;

      context.clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      boring_sampler.use(fb_unit);
      boring_modelview.set(glm::value_ptr(slow_rotated_modelview_matrix));
      context.draw(boring_vao, GL_TRIANGLE_STRIP, 26);

      app.update();
    }

  } catch(gl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

}
