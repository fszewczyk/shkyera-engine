#version 330 core

layout(location = 0) in vec3 aPos;  // Vertex position

uniform mat4 projectionViewMatrix;  // Model matrix
uniform mat4 modelMatrix;       // Projection matrix

out float VertexDepth;  // Output the depth value to the fragment shader

void main() {
  vec4 viewSpacePosition = projectionViewMatrix * modelMatrix * vec4(aPos, 1.0);
  gl_Position = viewSpacePosition;
  VertexDepth = viewSpacePosition.z;
}