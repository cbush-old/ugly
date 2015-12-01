#version 410 core

layout(quads, equal_spacing, ccw) in;

#define M_PI 3.1415926535897932384626433832795
#define M_PI_2 (M_PI / 2.0)

uniform mat4 projection;
uniform mat4 modelview;
uniform sampler2D texture_unit;

in vec2[] tc_texcoord;
out vec2 te_texcoord;

in vec4[] tc_normal;
out vec4 te_normal;

vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
  vec4 a = mix(v0, v1, gl_TessCoord.x);
  vec4 b = mix(v3, v2, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);
}

void main() {
  vec4 position = interpolate(
    gl_in[0].gl_Position,
    gl_in[1].gl_Position,
    gl_in[2].gl_Position,
    gl_in[3].gl_Position
  );

  te_texcoord = interpolate(
    vec4(tc_texcoord[0], 1.0, 1.0),
    vec4(tc_texcoord[1], 1.0, 1.0),
    vec4(tc_texcoord[2], 1.0, 1.0),
    vec4(tc_texcoord[3], 1.0, 1.0)
  ).xy;

  te_normal = interpolate(
    tc_normal[0],
    tc_normal[1],
    tc_normal[2],
    tc_normal[3]
  );

  vec4 nrml = te_normal;

  vec2 p = vec2(1.0 - abs(gl_TessCoord.x - 0.5) / 0.5, 1.0 - abs(gl_TessCoord.y - 0.5) / 0.5);
  float pv = p.x * p.y * 0.5;
  position += nrml * pv;

  vec4 tmp = modelview * position;
  const float fisheye_amount = 1.0;
  tmp.xy = tmp.xy * (1.0 - fisheye_amount) + (tmp.xy / length(tmp.xyz)) * fisheye_amount;
  gl_Position = projection * tmp;

}

