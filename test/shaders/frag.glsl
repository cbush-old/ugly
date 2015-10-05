#version 410

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D texture_unit;
uniform vec4 color;

in vec2 texcoord;

void main() {

  vec4 texel = texture(texture_unit, texcoord);

  if (texel.a < 0.1) {
    discard;
  }

  fragColor = texel;

}
