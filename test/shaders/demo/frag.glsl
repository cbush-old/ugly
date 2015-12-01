#version 410

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D texture_unit;

uniform vec4 ambient;
uniform vec4 light_color;
uniform vec3 light_direction;
uniform vec3 half_vector;
uniform float shininess;
uniform float strength;

in vec2 texcoord;
in vec3 normal;
in vec3 pos;

void main() {

  float diffuse = max(0.0, dot(normal, light_direction));
  float specular = max(0.0, dot(normal, half_vector));

  if (diffuse == 0.0) {
    specular = 0.0;
  } else {
    specular = pow(specular, shininess);
  }

  vec3 scattered = ambient.rgb + light_color.rgb * diffuse;
  vec3 reflected = light_color.rgb * specular * strength;

  vec3 mess = noise3(reflect(pos, normal));
  vec3 rgb = min(mess + texture(texture_unit, texcoord).rgb * scattered + reflected, vec3(1.0));
  vec4 texel = vec4(rgb, 1.f);

  fragColor = texel;

}
