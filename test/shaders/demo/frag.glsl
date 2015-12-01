#version 410

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D texture_unit;

uniform vec4 ambient;
uniform vec4 light_color;
uniform vec3 light_direction;
uniform float shininess;
uniform float strength;

in vec2 texcoord4;
in vec4 pos;
in vec3 surfacenormal;
uniform vec3 hv;

void main() {
  float distance = max(0.0, 1.0 - length(pos) / 500.0);
  vec4 texel = texture(texture_unit, texcoord4);
  texel.a = 1.0;

  float diffuse = max(0.0, dot(surfacenormal, light_direction));
  float fog = max(0.0, min(1.0, length(pos) / 1500.0));

  const vec4 reflect_color = vec4(0.9);

  float reflection = max(0.0, dot(vec3(0.0, 0.0, 1.0), surfacenormal));
  fragColor = max(vec4(0.0), min(
    mix(texel * (
        min(vec4(1.0), vec4(ambient.rgb * min(1.0, diffuse), 1.0))
      ) + reflect_color * pow(reflection, shininess),
      light_color,
      fog)
    , vec4(1.0)));
}
