#version 330 core

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 normal;    // Vertex normal

uniform mat4 modelMatrix;       // Model matrix
uniform mat4 viewMatrix;        // View matrix
uniform mat4 projectionMatrix;  // Projection matrix

out vec3 FragPos;  // Fragment position in world space
out vec3 Normal;   // Normal in world space

void main() {
  FragPos = vec3(modelMatrix *
                 vec4(position, 1.0));  // Transform position to world space
  Normal = normalize(mat3(transpose(inverse(modelMatrix))) *
                     normal);  // Transform normal to world space

  // Transform the vertex position to clip space
  gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
