#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D first;
uniform sampler2D second;

void main() {
  vec3 result = texture(first, TexCoords).rgb;
  result -= texture(second, TexCoords).rgb;

  FragColor = vec4(result, 1.0);
}
