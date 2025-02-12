#version 330 core

uniform sampler2D original;
uniform float gamma;

// ******** TRANSFORMATION DATA ********
out vec4 FragColor;
in vec2 TexCoords; 

vec3 gammaCorrection(vec3 color)
{
  return pow(color, vec3(1.0 / gamma));
}

void main() {
  vec3 color = gammaCorrection(texture(original, TexCoords).rgb);
  FragColor = vec4(color, 1.0);
}