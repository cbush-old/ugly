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
    MAKE3D(-1, +1, -1),
    MAKE3D(+1, +1, -1),

    // Front face
    0,0, 1,0, 0,1, 1,1,
    // Right face
    0,1, 0,0, 1,1, 1,0,
    // Back face
    0,0, 1,0, 0,1, 1,1,
    // Left face
    0,1, 0,0, 1,1, 1,0,
    // Bottom face
    0,1, 0,0, 1,1, 1,0,
    // Move to top
    1,0, 0,0,
    // Top face
    0,0, 1,0, 0,1, 1,1,

    // Front
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // Right
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // Back
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    // Left
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    // Bottom
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    // To top
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    // Top
    0.0f, +1.0f, 0.0f,
    0.0f, +1.0f, 0.0f,
    0.0f, +1.0f, 0.0f,
    0.0f, +1.0f, 0.0f,

  };

  #undef MAKE3D

  buffer.data(vertex_data, GL_STATIC_DRAW, GL_ARRAY_BUFFER);

  gl::attrib position (program, "position");
  gl::attrib texcoord (program, "texcoord_in");
  gl::attrib normal (program, "normal_in");

  vao.pointer(buffer, position, 3, GL_FLOAT, GL_FALSE, 0, 0);
  vao.pointer(buffer, texcoord, 2, GL_FLOAT, GL_FALSE, 0, 26 * 3 * sizeof(GLfloat));
  vao.pointer(buffer, normal, 3, GL_FLOAT, GL_FALSE, 0, 26 * 5 * sizeof(GLfloat));

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
      gl::FragmentShader("shaders/demo/frag.glsl")
    );

    gl::uniform4<float> ambient (program, program.uniform_location("ambient"));
    gl::uniform4<float> light_color (program, program.uniform_location("light_color"));
    gl::uniform3<float> light_direction (program, program.uniform_location("light_direction"));
    gl::uniform3<float> half_vector (program, program.uniform_location("half_vector"));
    gl::uniform<float> shininess (program, program.uniform_location("shininess"));
    gl::uniform<float> strength (program, program.uniform_location("strength"));

    ambient.set(0.2f, 0.2f, 0.2f, 1.f);
    light_color.set(1.f, 1.f, 1.f, 1.f);
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
    unsigned check_size = 64;
    for (size_t i = 0; i < pixels.size(); ++i) {
      uint8_t c = (((i % tw) / check_size) & 1) * 0xff;
      c = (((i / tw) / check_size) & 1) ? (c ? 0 : 0xff) : c;
      c >>= 1;
      c += 0x30;
      pixels[i] = uint32_t(i << 9);

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

    // Background color
    GL_CALL(glClearColor(0.5f, 0.2f, 0.3f, 1.f));


    // Properties of the displayed object
    float angle = 0.f;
    glm::vec3 position (0.f, 0.f, -2.5f);

    float light_angle = 1.f;
    glm::vec3 direction = glm::rotate(glm::vec3(0.f, 0.f, 1.f), light_angle, glm::vec3(0.f, 1.f, 0.f));
    light_direction.set(direction.x, direction.y, direction.z);

    glm::vec3 hv = glm::normalize((direction + glm::vec3(0.f, 0.f, 1.f)) / 2.f);
    half_vector.set(hv.x, hv.y, hv.z);

    while (!app.done()) {

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

      vao.draw(GL_TRIANGLE_STRIP, 26);
      app.update();
    }

  } catch(gl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

}
