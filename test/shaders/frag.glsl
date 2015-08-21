#version 410

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D texture_unit;
uniform vec4 color;
uniform vec3 ambient_color;
uniform vec3 ambient_direction;
uniform float ambient_intensity;
uniform vec4 bgcolor;
uniform vec3 normal;

in vec2 texcoord4;
in vec4 pos;
in vec3 surfacenormal;

void main() {

  vec4 texel = texture(texture_unit, texcoord4) * color;

  if (texel.a < 0.1) {
    discard;
  }

  float diffuse = max(0.0, dot(surfacenormal, -ambient_direction));
  float fog = max(0.0, min(1.0, pos.z / 5.f));

  fragColor = texel
    * vec4(ambient_color * min(1.0, (ambient_intensity + diffuse)), 1.0)
    * (1.0 - fog) + (fog * bgcolor);

}
