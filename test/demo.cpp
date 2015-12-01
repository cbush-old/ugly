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


int main(int argc, const char* const argv[]) {

  try {

    glfwApp app;

    gl::MonoContext context (&app);

    GL_CALL(glDisable(GL_CULL_FACE));
    GL_CALL(glEnable(GL_DEPTH_TEST));


    gl::Program program (
      context,
      gl::VertexShader("shaders/demo/vert.glsl"),
      gl::TessControlShader("shaders/demo/tess_control.glsl"),
      gl::TessEvaluationShader("shaders/demo/tess_eval.glsl"),
      gl::GeometryShader("shaders/demo/geo.glsl"),
      gl::FragmentShader("shaders/demo/frag.glsl")
    );

    gl::uniform4<float> ambient (program, program.uniform_location("ambient"));
    gl::uniform4<float> light_color (program, program.uniform_location("light_color"));
    gl::uniform3<float> light_direction (program, program.uniform_location("light_direction"));
    gl::uniform3<float> half_vector (program, program.uniform_location("half_vector"));
    gl::uniform<float> shininess (program, program.uniform_location("shininess"));
    gl::uniform<float> strength (program, program.uniform_location("strength"));

    ambient.set(0.5f, 0.5f, 0.5f, 1.f);
    light_color.set(0.5f, 1.f, 1.f, 1.f);
    shininess.set(20.f);
    strength.set(200.f);

    gl::uniform_mat4 modelview (program, program.uniform_location("modelview"));
    gl::uniform_mat4 normal_matrix (program, program.uniform_location("normal_matrix"));
    gl::uniform_mat4 projection (program, program.uniform_location("projection"));

    glm::mat4 projection_matrix {
      glm::perspective<GLfloat>(
        45,
        float(app.width()) / float(app.height()),
        1.0,
        20.0
      )
    };

    projection.set(glm::value_ptr(projection_matrix));


    gl::VertexArray vao;
    gl::Buffer buffer;
    set_vertex_data(buffer, vao, program);
    
    // Set up texture
    //
    //
    int tw = 16;
    std::vector<uint32_t> pixels (tw * tw);
    for (size_t i = 0; i < pixels.size(); ++i) {
      pixels[i] = rand();
    }
    
    gl::Texture2D texture;
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.name()));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    gl::ImageDesc2D desc (tw, tw, pixels.data());
    texture.image(0, desc);
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.name()));

    // Properties of the displayed object
    float angle = 0.f;
    glm::vec3 position (0.f, 0.f, -2.0f);

    float light_angle = 1.f;
    glm::vec3 direction = glm::rotate(glm::vec3(0.f, 0.f, 1.f), light_angle, glm::vec3(0.f, 1.f, 0.f));
    light_direction.set(direction.x, direction.y, direction.z);

    glm::vec3 hv = glm::normalize((direction + glm::vec3(0.f, 0.f, 1.f)) / 2.f);
    half_vector.set(hv.x, hv.y, hv.z);

    float tick = 0.f;
    while (!app.done()) {
      tick += 0.01f;
      context.clear_color(
        0.5f + sin(tick) * 0.5f,
        0.5f + sin(tick + 1.f) * 0.5f,
        0.5f + sin(tick + 2.f) * 0.5f
      );

      context.clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      glm::mat4 modelview_matrix {
        glm::translate(
          glm::mat4(),
          position
        )
      };

      angle += 0.015f;
      glm::mat4 rotation;
      rotation = glm::rotate(rotation, angle, glm::vec3(1.f, 0.f, 0.f));
      rotation = glm::rotate(rotation, angle * 0.5f, glm::vec3(0.f, 0.f, 1.f));
      rotation = glm::rotate(rotation, angle, glm::vec3(0.f, 1.f, 0.f));
      modelview_matrix = modelview_matrix * rotation;


      normal_matrix.set(glm::value_ptr(rotation));

      modelview.set(glm::value_ptr(modelview_matrix));

      vao.draw(GL_PATCHES, 24);
      app.update();
    }

  } catch(gl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

}
