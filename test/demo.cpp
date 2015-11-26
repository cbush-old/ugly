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


class VertexArray {
  public:
    VertexArray() {
      GL_CALL(glGenVertexArrays(1, &_name));
    }

    ~VertexArray() {
      GL_CALL(glDeleteVertexArrays(1, &_name));
    }

    void bind() {
      GL_CALL(glBindVertexArray(_name));
    }

  private:
    GLuint _name;
};


void set_vertex_data(GLuint vbo, GLuint program) {

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

  GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), vertex_data.data(), GL_STATIC_DRAW));

  GLint position = glGetAttribLocation(program, "position");
  GLint texcoord = glGetAttribLocation(program, "texcoord_in");
  GLint normal = glGetAttribLocation(program, "normal_in");

  std::printf("position: %d\ntexcoord: %d\nnormal: %d\n", position, texcoord, normal);

  size_t offset = 0;
  GL_CALL(glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset));
  offset += (26 * 3) * sizeof(GLfloat);
  GL_CALL(glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset));
  offset += (26 * 2) * sizeof(GLfloat);
  GL_CALL(glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset));

  GL_CALL(glEnableVertexAttribArray(position));
  GL_CALL(glEnableVertexAttribArray(texcoord));
  GL_CALL(glEnableVertexAttribArray(normal));


}


int main(int argc, const char* const argv[]) {

  GLuint texture;

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

    auto modelview = program.uniform_location("modelview");
    auto normal_matrix = program.uniform_location("normal_matrix");
    auto projection = program.uniform_location("projection");

    glm::mat4 projection_matrix {
      glm::perspective<GLfloat>(
        45,
        float(app.width()) / float(app.height()),
        1.0,
        20.0
      )
    };

    GL_CALL(glUniformMatrix4fv(
      projection,
      1,
      GL_FALSE,
      glm::value_ptr(projection_matrix)
    ));


    VertexArray vao;
    vao.bind();


    GLuint vbo;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    set_vertex_data(vbo, program.name());



    // Set up texture
    //
    //
    int tw = 512;
    std::vector<uint32_t> pixels (tw * tw);
    unsigned check_size = 64;
    for (size_t i = 0; i < pixels.size(); ++i) {
      uint8_t c = (((i % tw) / check_size) & 1) * 0xff;
      c = (((i / tw) / check_size) & 1) ? (c ? 0 : 0xff) : c;
      c >>= 1;
      c += 0x30;

      pixels[i] =
        i << 9;/*
        (c << 24) |
        (c << 16) |
        (c << 8) |
        0xff;*/

    }
    GL_CALL(glGenTextures(1, &texture));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      GL_RGBA, 
      tw,
      tw,
      0,
      GL_RGBA,
      GL_UNSIGNED_INT_8_8_8_8, 
      (const GLvoid*)pixels.data()
    ));



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


      GL_CALL(glUniformMatrix4fv(
        normal_matrix,
        1,
        GL_FALSE,
        glm::value_ptr(rotation)
      ));

      GL_CALL(glUniformMatrix4fv(
        modelview,
        1,
        GL_FALSE,
        glm::value_ptr(modelview_matrix)
      ));

      GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 26));
      app.update();
    }

  } catch(gl::exception const& e) {
    std::cout << e.what() << std::endl;
  }

  GL_CALL(glDeleteTextures(1, &texture));

}
