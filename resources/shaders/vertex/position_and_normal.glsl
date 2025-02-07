#version 330 core

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 normal;    // Vertex normal
layout(location = 2) in vec2 texCoord;  // Texture coordinates
layout(location = 3) in vec3 tangent;  // Tangent Vector

uniform mat4 modelMatrix;       // Model matrix
uniform mat4 projectionViewMatrix;        // View matrix

out vec3 FragPos;  // Fragment position in world space
out vec3 Normal;   // Normal in world space
out vec2 UV;
out vec3 Tangent;

void main() {
  // Transform position and normal to world space
  FragPos = vec3(modelMatrix * vec4(position, 1.0));
  Normal = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
  Tangent = normalize(mat3(modelMatrix) * tangent);

  UV = texCoord;

  // Set final vertex position
  gl_Position = projectionViewMatrix * vec4(FragPos, 1.0);
}
