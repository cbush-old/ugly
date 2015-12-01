#version 410

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normal_matrix;

in vec4 position;
in vec2 texcoord_in;
in vec4 normal_in;

out vec2 vs_texcoord;
out vec4 vs_normal;

void main() {
  vs_texcoord = texcoord_in;
  vs_normal = normal_in;
  gl_Position = position;
}

