#version 330 core

layout(location = 0) in vec3 position;
out vec3 texCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
  texCoords = position;
  vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(position, 1.0);
  gl_Position = pos.xyww;
}