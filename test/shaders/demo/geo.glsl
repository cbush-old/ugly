#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2[] te_texcoord;
in vec4[] te_normal;

out vec2 texcoord4;
out vec3 surfacenormal;
out vec4 pos;

void main(void)
{
  vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
  vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
  vec3 nrml = normalize(cross(a, b));

  for (int i = 0; i < 3; ++i) {
    surfacenormal = nrml;
    pos = gl_in[i].gl_Position;
    texcoord4 = te_texcoord[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();

}

