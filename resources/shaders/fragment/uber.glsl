#version 330 core

out vec4 FragColor;  // Final fragment color

in vec3 FragPos;  // Fragment position in world space
in vec3 Normal;   // Normal in world space

uniform vec3 viewPos;  // Camera position

struct PointLight {
  vec3 position;  // Light position
  vec3 diffuse;   // Diffuse light color
  vec3 specular;  // Specular light color
};

struct DirectionalLight {
  vec3 direction;  // Light position
  vec3 diffuse;    // Diffuse light color
  vec3 specular;   // Specular light color
};

uniform vec3 ambientLight;

uniform int numPointLights;
uniform PointLight pointLights[16];

uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[16];

// Material structure
struct Material {
  vec3 diffuse;     // Diffuse material color
  vec3 specular;    // Specular material color
  float shininess;  // Shininess factor
};

uniform Material material;

void main() {
  vec3 result = ambientLight * material.diffuse;

  for (int i = 0; i < numPointLights; ++i) {
    // Diffuse
    vec3 lightDir = normalize(pointLights[i].position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = pointLights[i].diffuse * diff * material.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLights[i].specular * spec * material.specular;

    result += diffuse + specular;
  }

  for (int i = 0; i < numDirectionalLights; ++i) {
    // Diffuse
    vec3 lightDir = directionalLights[i].direction;
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = directionalLights[i].diffuse * diff * material.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLights[i].specular * spec * material.specular;

    result += diffuse + specular;
  }

  FragColor = vec4(result, 1.0);
}
