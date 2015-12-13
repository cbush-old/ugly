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
  diffuse = min(1.0, diffuse);

  float reflection = max(0.0, dot(surfacenormal, hv));
  reflection = min(1.0, reflection);
  reflection = pow(reflection, shininess) * strength;
  
  vec4 reflect_color = vec4(reflection);

  fragColor = max(vec4(0.0), min(vec4(1.0),
    texel * (
      min(vec4(1.0), vec4(ambient.rgb * diffuse, 1.0))
    )
    +
    reflect_color
  ));

}
