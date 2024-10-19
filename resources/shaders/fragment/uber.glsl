#version 330 core

out vec4 FragColor;  // Final fragment color

in vec3 FragPos;  // Fragment position in world space
in vec3 Normal;   // Normal in world space

uniform vec3 viewPos;  // Camera position

// Light structure
struct Light {
  vec3 position;  // Light position
  vec3 ambient;   // Ambient light color
  vec3 diffuse;   // Diffuse light color
  vec3 specular;  // Specular light color
};

uniform int numLights;     // Number of active lights
uniform Light lights[16];  // Array of lights (supporting up to 16 point lights)

// Material structure
struct Material {
  vec3 diffuse;     // Diffuse material color
  vec3 specular;    // Specular material color
  float shininess;  // Shininess factor
};

uniform Material material;  // Material properties

void main() {
  vec3 result = vec3(0.0);  // Initialize lighting result

  // Process each light source
  for (int i = 0; i < numLights; ++i) {
    // Ambient component
    vec3 ambient = lights[i].ambient * material.diffuse;

    // Diffuse component
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * diff * material.diffuse;

    // Specular component
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[i].specular * spec * material.specular;

    // Accumulate the lighting
    result += ambient + diffuse + specular;
  }

  FragColor =
      vec4(result, 1.0);  // Set the final fragment color with full opacity
}
