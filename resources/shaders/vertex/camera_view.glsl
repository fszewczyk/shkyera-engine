#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;  // Color output for Gouraud shading

// Light structure
struct Light {
  vec3 position;  // Light position
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

// Material structure
struct Material {
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform int numLights;     // Number of active lights
uniform Light lights[16];  // Array of lights (supporting up to 16 point lights)
uniform Material material;  // Material properties
uniform vec3 viewPos;       // Camera position

void main() {
  FragPos = vec3(modelMatrix * vec4(position, 1.0));  // World space position
  Normal = normalize(mat3(transpose(inverse(modelMatrix))) *
                     normal);  // Normal in world space

  // Initialize lighting result for Gouraud shading
  vec3 result = vec3(0.0);
  vec3 viewDir =
      normalize(viewPos - FragPos);  // Direction from fragment to view

  // Process each light
  for (int i = 0; i < numLights; ++i) {
    vec3 ambient = lights[i].ambient * material.diffuse;

    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * diff * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[i].specular * spec * material.specular;

    result += ambient + diffuse + specular;  // Accumulate the lighting
  }

  VertexColor = result;  // Pass the computed color to the fragment shader

  gl_Position =
      projectionMatrix * viewMatrix * vec4(FragPos, 1.0);  // MVP transform
}
