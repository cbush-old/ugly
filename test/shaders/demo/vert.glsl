#version 410

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normal_matrix;

in vec4 position;
in vec2 texcoord_in;
in vec4 normal_in;

out vec3 pos;
out vec2 texcoord;
out vec3 normal;

void main() {

  vec4 tmp = modelview * position;

  pos = (modelview * position).xyz;

  pos.xy = tmp.xy / length(tmp.xyz);
  texcoord = texcoord_in;
  normal = normalize(normal_matrix * normal_in).xyz;
  gl_Position = projection * vec4(pos, 1.0);

}
