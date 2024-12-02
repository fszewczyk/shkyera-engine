#version 330 core

layout(location = 0) in vec3 position;  // Vertex position

uniform mat4 projectionViewModelMatrix;       // Model matrix

void main() {
  // Transform the vertex position to clip space
  gl_Position = projectionViewModelMatrix * vec4(position, 1.0);
}
