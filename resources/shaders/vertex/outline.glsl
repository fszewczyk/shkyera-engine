#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float outlineWidth;  // width in screen space

void main() {
  // Transform the vertex position to world space
  vec4 worldPosition = modelMatrix * vec4(inPosition, 1.0);
  vec3 worldNormal =
      normalize(mat3(transpose(inverse(modelMatrix))) * inNormal);

  // Transform to view space and then to screen space
  vec4 viewPosition = viewMatrix * worldPosition;
  vec4 projectedPosition = projectionMatrix * viewPosition;

  // Calculate screen-space offset
  vec3 screenOffset = normalize(mat3(viewMatrix) * worldNormal) * outlineWidth;
  vec4 outlinePosition = projectedPosition + vec4(screenOffset, 0.0);

  // Final position for outline
  gl_Position = outlinePosition;
}
