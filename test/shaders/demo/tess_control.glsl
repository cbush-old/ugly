#version 410 core

layout(vertices = 4) out;

in vec4[] vs_normal;
in vec2[] vs_texcoord;
out vec2[] tc_texcoord;
out vec4[] tc_normal;

void main(void) {
  float amount = 8;
  gl_TessLevelInner[0] = amount;
  gl_TessLevelInner[1] = amount;
  gl_TessLevelOuter[0] = amount;
  gl_TessLevelOuter[1] = amount;
  gl_TessLevelOuter[2] = amount;
  gl_TessLevelOuter[3] = amount;
  tc_texcoord[gl_InvocationID] = vs_texcoord[gl_InvocationID];
  tc_normal[gl_InvocationID] = vs_normal[gl_InvocationID];
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

