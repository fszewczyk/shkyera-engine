#version 330 core

in vec3 FragPos;  // Fragment position

uniform vec3 outlineColor;  // Color of the outline

out vec4 FragColor;  // Final output color of the fragment

void main() {
  FragColor = vec4(outlineColor, 1.0);  // Set fragment color to outline color
}
