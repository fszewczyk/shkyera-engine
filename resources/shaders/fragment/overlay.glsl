#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D background;
uniform sampler2D overlay;

void main() {
  vec3 overlayFragColor = texture(overlay, TexCoords).rgb;
  vec3 backgroundFragColor = texture(background, TexCoords).rgb;
  if (overlayFragColor == vec3(0, 0, 0)) {
    FragColor = vec4(backgroundFragColor, 1.0);
  } else {
    FragColor = vec4(overlayFragColor, 1.0);
  }
}
