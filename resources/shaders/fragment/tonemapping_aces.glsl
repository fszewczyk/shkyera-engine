#version 330 core

uniform sampler2D original;

// ******** TRANSFORMATION DATA ********
out vec4 FragColor;
in vec2 TexCoords; 

// ******** FUNCTIONS ********
vec3 toneMappingACES(vec3 color) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (color * (a * color + b)) / (color * (c * color + d) + e);
}

vec3 gammaCorrection(vec3 color)
{
  return pow(color, vec3(1.0 / 1.2));
}

void main() {
  // Lighting
  vec3 color = toneMappingACES(texture(original, TexCoords).rgb);
  color = gammaCorrection(color);
  FragColor = vec4(color, 1.0);
}