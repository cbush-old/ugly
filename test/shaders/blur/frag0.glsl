#version 410

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D texture_unit;

in vec2 texcoord;


void main() {
  vec4 texel;
  float space = 0.0005;
  float top = 30;
  float start = -top;
  for (float i = start; i < top; ++i) {
    float f = 0.5 - abs(i / top) / 2.0;
    texel += 0.1 * f * texture(texture_unit, texcoord + vec2(i * space, 0.0));
  }
  fragColor = texel;
}
