#version 330 core

layout(location = 1) in vec3 normal;    // Vertex normal

uniform mat4 modelMatrix;       // Model matrix
uniform mat4 projectionViewMatrix;        // View matrix

out vec3 Normal;   // Normal in world space

void main() {
  vec3 fragPos = vec3(modelMatrix * vec4(position, 1.0));
  Normal = normalize(mat3(transpose(inverse(modelMatrix))) * normal);

  gl_Position = projectionViewMatrix * vec4(fragPos, 1.0);
}
