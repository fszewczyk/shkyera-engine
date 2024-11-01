#version 330 core

layout(location = 0) in vec3 position;  // Vertex position

uniform mat4 modelMatrix;       // Model matrix
uniform mat4 viewMatrix;        // View matrix
uniform mat4 projectionMatrix;  // Projection matrix

void main() {
  // Transform the vertex position to clip space
  gl_Position =
      projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
