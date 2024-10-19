#version 330 core

out vec4 FragColor;

in vec3 VertexColor;  // Interpolated color from the vertex shader

void main() {
  FragColor = vec4(VertexColor, 1.0);  // Output the interpolated color
}
