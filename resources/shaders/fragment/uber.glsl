#version 330 core

// ******** TRANSFORMATION DATA ********
out vec4 FragColor;  

in vec3 FragPos; 
in vec3 Normal;  

uniform vec3 viewPos;  // Camera position

// ******** LIGHT DATA ********
#define POINT_LIGHT_MAX_NUM 4
struct PointLight {
  vec3 position; 
  vec3 color;  
  float range;
};

uniform int numPointLights;
uniform PointLight pointLights[POINT_LIGHT_MAX_NUM];
uniform sampler2D pointLightsDepthCubeMap[POINT_LIGHT_MAX_NUM];

#define DIRECTIONAL_LIGHT_LODS 4
#define DIRECTIONAL_LIGHT_MAX_NUM 4
struct DirectionalLight {
  mat4 lightSpaceMatrix[DIRECTIONAL_LIGHT_LODS];
  vec3 direction; 
  vec3 color;    
};
uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHT_MAX_NUM];
uniform sampler2D directionalLightsDepthMap[DIRECTIONAL_LIGHT_MAX_NUM];

#define SPOT_LIGHT_MAX_NUM 4
struct SpotLight {
  mat4 lightSpaceMatrix;
  vec3 position;
  vec3 direction;
  vec3 color;
  float range;
  float innerCutoffCosine; // Cosine of inner cutoff angle
  float outerCutoffCosine; // Cosine of outer cutoff angle
};

uniform int numSpotLights;
uniform SpotLight spotLights[SPOT_LIGHT_MAX_NUM];
uniform sampler2D spotLightsShadowMap[SPOT_LIGHT_MAX_NUM];

uniform vec3 ambientLight;

// ******** MATERIAL DATA ********
struct Material {
  vec3 color;     
  float shininess;  
};
uniform Material material;

// ******** POST-PROCESSING DATA ********
uniform float oneOverGamma;

// ******** FUNCTIONS ********
vec3 toneMappingACES(vec3 color) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (color * (a * color + b)) / (color * (c * color + d) + e);
}

vec3 gammaCorrection(vec3 color)
{
  return pow(color, vec3(oneOverGamma));
}

float sampleAtlas(sampler2D map, int cols, int index, vec2 xy)
{
  return texture(map, vec2((1.0 / cols) * index + 0.0005, 0) + xy * vec2((1.0 / cols) - 0.001, 1)).r;
}

float sampleCubeMap(sampler2D map, vec3 direction)
{
  vec2 uv;
  int faceIndex;
  if (abs(direction.x) > abs(direction.y) && abs(direction.x) > abs(direction.z)) {
    faceIndex = direction.x > 0.0 ? 1 : 0;
    if(faceIndex == 0)
    {
      uv = vec2(direction.z, direction.y) / abs(direction.x);
    }
    else
    {
      uv = vec2(-direction.z, direction.y) / abs(direction.x);
    }
  } else if (abs(direction.y) > abs(direction.x) && abs(direction.y) > abs(direction.z)) {
    faceIndex = direction.y > 0.0 ? 3 : 2;
    if(faceIndex == 3)
    {
      uv = vec2(-direction.x, direction.z) / abs(direction.y);
    }
    else
    {
      uv = vec2(-direction.x, -direction.z) / abs(direction.y);
    }
  } else {
    faceIndex = direction.z > 0.0 ? 5 : 4;
    if(faceIndex == 5)
    {
      uv = vec2(direction.x, direction.y) / abs(direction.z);
    }
    else
    {
      uv = vec2(-direction.x, direction.y) / abs(direction.z);
    }
  }

  uv = uv * 0.5 + 0.5;

  return sampleAtlas(map, 6, faceIndex, uv);
}

vec3 calculateAmbient() {
  return ambientLight * material.color;
}

float calculateSpotFalloff(vec3 lightDir, vec3 spotlightDir, float innerCutoffCosine, float outerCutoffCosine) {
  float theta = dot(lightDir, normalize(-spotlightDir));
  float epsilon = innerCutoffCosine - outerCutoffCosine;
  return clamp((theta - outerCutoffCosine) / epsilon, 0.0, 1.0);
}

vec3 calculateSpotLights() {
  vec3 normal = normalize(Normal);
  vec3 result = vec3(0.0);

  for (int i = 0; i < numSpotLights; ++i) {
    vec3 lightToFrag = FragPos - spotLights[i].position;
    float distanceToLight = length(lightToFrag);

    float attenuation = 1.0 - (distanceToLight / spotLights[i].range);

    if (attenuation > 0.0) {
      vec3 lightDir = normalize(-lightToFrag);
      float falloff = calculateSpotFalloff(lightDir, spotLights[i].direction, spotLights[i].innerCutoffCosine, spotLights[i].outerCutoffCosine);

      if (falloff > 0.0) {
        // Shadow calculation
        vec4 fragPosLightSpace = spotLights[i].lightSpaceMatrix * vec4(FragPos, 1.0);
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;

        // Check if fragment is outside the shadow map's range
        if (projCoords.z > 1.0) {
          continue;
        }

        // Percentage-closer filtering
        float shadow = 0.0;
        float bias = 0.005;  // Bias to reduce shadow acne
        int pcfKernelSize = 3;
        float texelSize = 1.0 / 1024.0; // Assuming 1024x1024 shadow map resolution

        for (int x = -pcfKernelSize; x <= pcfKernelSize; ++x) {
          for (int y = -pcfKernelSize; y <= pcfKernelSize; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float shadowMapDepth = texture(spotLightsShadowMap[i], projCoords.xy + offset).r;
            if (distanceToLight - bias <= shadowMapDepth) {
              shadow += 1.0;
            }
          }
        }

        int totalSamples = (2 * pcfKernelSize + 1) * (2 * pcfKernelSize + 1);
        shadow /= float(totalSamples);

        // Diffuse lighting
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = spotLights[i].color * diff * material.color;

        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spotLights[i].color * spec * material.color;

        result += attenuation * falloff * (diffuse + specular) * shadow;
      }
    }
  }

  return result;
}

vec3 calculatePointLights() {
  vec3 normal = normalize(Normal);
  vec3 result = vec3(0.0);

  for (int i = 0; i < numPointLights; ++i) {
    vec3 lightToFrag = FragPos - pointLights[i].position;
    float distanceToLight = length(lightToFrag);
    float relativeIntensity = 1.0 - (distanceToLight / pointLights[i].range);

    if (relativeIntensity > 0.0) {
      // Calculate light direction
      vec3 lightDir = normalize(-lightToFrag);

      // Perform PCF
      float bias = 0.1; // Bias to reduce shadow acne
      float shadow = 0.0;
      int pcfSamples = 1; // Number of PCF samples per dimension
      float pcfRadius = 0.005; // Radius for PCF sampling

      for (int x = -pcfSamples; x <= pcfSamples; ++x) {
        for (int y = -pcfSamples; y <= pcfSamples; ++y) {
          for (int z = -pcfSamples; z <= pcfSamples; ++z) {
            vec3 offsetDir = lightDir + vec3(x, y, z) * pcfRadius;
            float shadowMapDepth = sampleCubeMap(pointLightsDepthCubeMap[i], offsetDir);
            if (distanceToLight - bias <= shadowMapDepth) {
              shadow += 1.0;
            }
          }
        }
      }

      // Normalize shadow value
      int totalSamples = (2 * pcfSamples + 1) * (2 * pcfSamples + 1) * (2 * pcfSamples + 1);
      shadow /= float(totalSamples);

      // Diffuse lighting
      vec3 lightDirNormalized = normalize(lightDir);
      float diff = max(dot(normal, lightDirNormalized), 0.0);
      vec3 diffuse = pointLights[i].color * diff * material.color;

      // Specular lighting
      vec3 viewDir = normalize(viewPos - FragPos);
      vec3 reflectDir = reflect(-lightDirNormalized, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular = pointLights[i].color * spec * material.color;

      // Apply lighting with shadows
      result += shadow * relativeIntensity * (diffuse + specular);
    }
  }

  return result;
}


float calculateShadowFactor(int directionalLightIndex, int cascadeIndex) {
  vec4 fragPosLightSpaceHighLod = directionalLights[directionalLightIndex].lightSpaceMatrix[cascadeIndex] * vec4(FragPos, 1.0);
  vec3 projCoordsHighLod = fragPosLightSpaceHighLod.xyz / fragPosLightSpaceHighLod.w;
  projCoordsHighLod = projCoordsHighLod * 0.5 + 0.5;

  if (projCoordsHighLod.z > 1.0) {
    return 1.0;
  }

  float bias = 0.01;
  int pcfKernelSize = 3;

  float texelSizeHighLod =  1 / 1024.0;

  float shadowHighLod = 0.0;
  for (int x = -pcfKernelSize; x <= pcfKernelSize; ++x) {
    for (int y = -pcfKernelSize; y <= pcfKernelSize; ++y) {
      float closestDepthHighLod = sampleAtlas(directionalLightsDepthMap[directionalLightIndex], DIRECTIONAL_LIGHT_LODS, cascadeIndex, (projCoordsHighLod.xy + vec2(x, y) * texelSizeHighLod)) * 0.5 + 0.5;
      if (projCoordsHighLod.z - bias > closestDepthHighLod) {
        shadowHighLod += 1.0;
      }
    }
  }
  shadowHighLod /= float((pcfKernelSize * 2 + 1) * (pcfKernelSize * 2 + 1));

  return 1 - shadowHighLod;
}

int chooseLightCascade(mat4 lightSpaceMatrix[DIRECTIONAL_LIGHT_LODS], vec3 fragPos) {
  const float blendRange = 1.0;  // Range near cascade far edge to blend

  for (int lod = 0; lod < DIRECTIONAL_LIGHT_LODS; ++lod) {
    vec4 fragPosLightSpace = lightSpaceMatrix[lod] * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x >= 0.0 && projCoords.y >= 0.0 && projCoords.x <= 1.0 &&
        projCoords.y <= 1.0 && projCoords.z < 1.0) {
      return lod;
    }
  }

  return -1;
}

vec3 calculateDirectionalLights() {
  vec3 normal = normalize(Normal);
  vec3 result = vec3(0.0);
  for (int i = 0; i < numDirectionalLights; ++i) {
    vec3 lightDir = normalize(directionalLights[i].direction);

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLights[i].color * diff * material.color;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLights[i].color * spec * material.color;

    // Calculate shadow factor
    int cascadeIndex = chooseLightCascade(directionalLights[i].lightSpaceMatrix, FragPos);
    float shadow = 1.0;
    if (cascadeIndex != -1) {
      shadow = calculateShadowFactor(i, cascadeIndex);
    }
    result += (diffuse + specular) * shadow;
  }
  return result;
}

void main() {
  // Lighting
  vec3 color = calculateAmbient();
  color += calculatePointLights();
  color += calculateDirectionalLights();
  color += calculateSpotLights();

  // Post-Processing
  color = toneMappingACES(color);
  color = gammaCorrection(color);
  FragColor = vec4(color, 1.0);
}