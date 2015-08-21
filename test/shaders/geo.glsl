#version 410 core

layout(triangles, invocations = 1) in;
//layout(line_strip, max_vertices = 4) out;
layout(triangle_strip, max_vertices = 4) out;

in vec2[] texcoord3;
out vec2 texcoord4;
out vec3 surfacenormal;
out vec4 pos;

void main(void)
{
  texcoord4 = texcoord3[0];
  vec4 a = gl_in[1].gl_Position - gl_in[0].gl_Position;
  vec4 b = gl_in[2].gl_Position - gl_in[0].gl_Position;
  vec4 nrml = a * b;
  vec3 a3 = vec3(a.x, a.y, a.z);
  vec3 b3 = vec3(b.x, b.y, b.z);
  surfacenormal = vec3(nrml.x, nrml.y, nrml.z);
  pos = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
  
  for (int i = 0; i < 3; ++i) {
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}
