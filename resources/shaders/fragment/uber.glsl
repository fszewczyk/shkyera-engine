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
  sampler2D shadowSampler[4];
  mat4 lightSpaceMatrix[4];
  vec3 direction;  // Light position
  vec3 color;      // Diffuse light color
};

struct LodBlend {
  int highLod;
  int lowLod;
  float highLodWeight;
};

uniform vec3 ambientLight;

uniform int numPointLights;
uniform PointLight pointLights[16];

uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[2];

// Material structure
struct Material {
  vec3 color;       // Diffuse material color
  float shininess;  // Shininess factor
};

uniform Material material;

vec3 calculateAmbient() {
  return ambientLight * material.color;
}

vec3 calculatePointLights() {
  vec3 normal = normalize(Normal);
  vec3 result = vec3(0.0);
  for (int i = 0; i < numPointLights; ++i) {
    float relativeIntensity = 1.0 - (length(pointLights[i].position - FragPos) /
                                     pointLights[i].range);

    if (relativeIntensity > 0) {
      // Diffuse
      vec3 lightDir = normalize(pointLights[i].position - FragPos);
      float diff = max(dot(normal, lightDir), 0.0);
      vec3 diffuse = pointLights[i].color * diff * material.color;

      // Specular
      vec3 viewDir = normalize(viewPos - FragPos);
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular = pointLights[i].color * spec * material.color;

      result += relativeIntensity * (diffuse + specular);
    }
  }
  return result;
}

// Calculate the shadow factor with PCF and LOD blending
float calculateShadowFactor(int directionalLightIndex, LodBlend lodBlend) {
  // Transform fragment position to light space for low LOD
  vec4 fragPosLightSpaceLowLod = directionalLights[directionalLightIndex]
                                     .lightSpaceMatrix[lodBlend.lowLod] *
                                 vec4(FragPos, 1.0);
  vec3 projCoordsLowLod =
      fragPosLightSpaceLowLod.xyz / fragPosLightSpaceLowLod.w;
  projCoordsLowLod = projCoordsLowLod * 0.5 + 0.5;  // Transform to [0, 1] range

  // Transform fragment position to light space for high LOD
  vec4 fragPosLightSpaceHighLod = directionalLights[directionalLightIndex]
                                      .lightSpaceMatrix[lodBlend.highLod] *
                                  vec4(FragPos, 1.0);
  vec3 projCoordsHighLod =
      fragPosLightSpaceHighLod.xyz / fragPosLightSpaceHighLod.w;
  projCoordsHighLod =
      projCoordsHighLod * 0.5 + 0.5;  // Transform to [0, 1] range

  // If outside the light's frustum, not in shadow
  if (projCoordsLowLod.z > 1.0 || projCoordsHighLod.z > 1.0) {
    return 1.0;
  }

  // Define PCF kernel size and bias for shadow comparison
  float bias = 0.01;
  int pcfKernelSize = 3;

  // Set texel sizes for low and high LODs
  float texelSizeLowLod =
      1.0 / 2048.0;  // Low LOD texel size (assumed 1024x1024)
  float texelSizeHighLod =
      2 * texelSizeLowLod;  // High LOD texel size, twice as large

  if (lodBlend.highLod == lodBlend.lowLod) {
    texelSizeHighLod = texelSizeLowLod;
  }

  // Calculate shadow factor for low LOD
  float shadowLowLod = 0.0;
  for (int x = -pcfKernelSize; x <= pcfKernelSize; ++x) {
    for (int y = -pcfKernelSize; y <= pcfKernelSize; ++y) {
      // Sample shadow map for low LOD
      float closestDepthLowLod =
          texture(directionalLights[directionalLightIndex]
                      .shadowSampler[lodBlend.lowLod],
                  projCoordsLowLod.xy + vec2(x, y) * texelSizeLowLod)
              .r;
      closestDepthLowLod =
          closestDepthLowLod * 0.5 + 0.5;  // Remap to [0, 1] range
      if (projCoordsLowLod.z - bias > closestDepthLowLod) {
        shadowLowLod += 1.0;
      }
    }
  }
  shadowLowLod /= float((pcfKernelSize * 2 + 1) * (pcfKernelSize * 2 + 1));

  // Calculate shadow factor for high LOD
  float shadowHighLod = 0.0;
  for (int x = -pcfKernelSize; x <= pcfKernelSize; ++x) {
    for (int y = -pcfKernelSize; y <= pcfKernelSize; ++y) {
      // Sample shadow map for high LOD
      float closestDepthHighLod =
          texture(directionalLights[directionalLightIndex]
                      .shadowSampler[lodBlend.highLod],
                  projCoordsHighLod.xy + vec2(x, y) * texelSizeHighLod)
              .r;
      closestDepthHighLod =
          closestDepthHighLod * 0.5 + 0.5;  // Remap to [0, 1] range
      if (projCoordsHighLod.z - bias > closestDepthHighLod) {
        shadowHighLod += 1.0;
      }
    }
  }
  shadowHighLod /= float((pcfKernelSize * 2 + 1) * (pcfKernelSize * 2 + 1));

  // Blend the shadow factors between the low and high LOD based on highLodWeight
  return mix(1.0 - shadowLowLod, 1.0 - shadowHighLod, lodBlend.highLodWeight);
}

LodBlend chooseLightLOD(mat4 lightSpaceMatrix[4], vec3 fragPos) {
  const float blendRange = 0.2;  // Range near cascade far edge to blend

  for (int lod = 0; lod < 4; ++lod) {
    vec4 fragPosLightSpace = lightSpaceMatrix[lod] * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Check if fragment is inside current LOD's view
    if (projCoords.x >= 0.0 && projCoords.y >= 0.0 && projCoords.x <= 1.0 &&
        projCoords.y <= 1.0 && projCoords.z < 1.0) {

      // If we're at the last LOD, no blending with a higher LOD
      if (lod == 3) {
        return LodBlend(lod, lod, 1.0);
      }

      // Transform to next LOD’s light space to calculate depth blending
      vec4 fragPosLightSpaceNext =
          lightSpaceMatrix[lod + 1] * vec4(fragPos, 1.0);
      vec3 projCoordsNext = fragPosLightSpaceNext.xyz / fragPosLightSpaceNext.w;
      projCoordsNext = projCoordsNext * 0.5 + 0.5;

      // Verify that fragment is also within next LOD's frustum
      if (projCoordsNext.x >= 0.0 && projCoordsNext.y >= 0.0 &&
          projCoordsNext.x <= 1.0 && projCoordsNext.y <= 1.0 &&
          projCoordsNext.z < 1.0) {

        // Compute blending weight based on fragment's depth in current LOD
        float depthInCurrentLOD = projCoords.z;
        float highLodWeight =
            smoothstep(1.0 - blendRange, 1.0, depthInCurrentLOD);

        return LodBlend(lod, lod + 1, highLodWeight);
      }

      // If fragment not inside next LOD, return current LOD without blending
      return LodBlend(lod, lod, 1.0);
    }
  }

  // Default return if fragment is outside all cascades
  return LodBlend(-1, -1, 0.0);
}

// Calculates the lighting from directional lights, including shadow mapping
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
    LodBlend lodBlend =
        chooseLightLOD(directionalLights[i].lightSpaceMatrix, FragPos);
    float shadow = 1.0;
    if (lodBlend.lowLod != -1) {
      shadow = calculateShadowFactor(i, lodBlend);
    }
    result += (diffuse + specular) * shadow;
  }
  return result;
}

void main() {
  vec3 ambient = calculateAmbient();
  vec3 pointLightResult = calculatePointLights();
  vec3 directionalLightResult = calculateDirectionalLights();

  vec3 result = ambient + pointLightResult + directionalLightResult;
  FragColor = vec4(result, 1.0);
}