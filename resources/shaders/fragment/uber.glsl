#version 330 core

out vec4 FragColor;  // Final fragment color

in vec3 FragPos;  // Fragment position in world space
in vec3 Normal;   // normal in world space

uniform vec3 viewPos;  // Camera position

struct PointLight {
  vec3 position;  // Light position
  vec3 color;     // Diffuse light color
  float range;    // Specular light color
};

struct DirectionalLight {
  vec3 direction;  // Light position
  vec3 color;      // Diffuse light color
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
  vec3 normal = normalize(Normal);

  for (int i = 0; i < numPointLights; ++i) {
    float relativeIntensity = 1.0 - (length(pointLights[i].position - FragPos) /
                                     pointLights[i].range);

    if (relativeIntensity > 0) {
      // Diffuse
      vec3 lightDir = normalize(pointLights[i].position - FragPos);
      float diff = max(dot(normal, lightDir), 0.0);
      vec3 diffuse = pointLights[i].color * diff * material.diffuse;

      // Specular
      vec3 viewDir = normalize(viewPos - FragPos);
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular = pointLights[i].color * spec * material.specular;

      result += relativeIntensity * (diffuse + specular);
    }
  }

  for (int i = 0; i < numDirectionalLights; ++i) {
    // Diffuse
    vec3 lightDir = directionalLights[i].direction;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLights[i].color * diff * material.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLights[i].color * spec * material.specular;

    result += diffuse + specular;
  }

  FragColor = vec4(result, 1.0);
}
