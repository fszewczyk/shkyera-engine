#version 330 core

layout(location = 0) in vec3 aPos;  // Vertex position

uniform mat4 modelMatrix;       // Model matrix
uniform mat4 viewMatrix;        // View matrix
uniform mat4 projectionMatrix;  // Projection matrix

out float FragDepth;  // Output the depth value to the fragment shader

void main() {
  // Transform vertex position to world space, then to light space
  vec4 lightSpacePosition =
      projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);

  // Set the final position
  gl_Position = lightSpacePosition;

  // Calculate the depth in light space (NDC) range [0,1]
  FragDepth = lightSpacePosition.z / lightSpacePosition.w;
}
