#version 330 core

out vec4 FragColor;

uniform vec3 fixedColor;

void main() {
  FragColor = vec4(fixedColor, 1.0);
}
