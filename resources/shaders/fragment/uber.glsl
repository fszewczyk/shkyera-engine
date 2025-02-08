#version 330 core

// ******** TRANSFORMATION DATA ********
out vec4 FragColor;  

in vec3 FragPos; 
in vec3 Normal;  
in vec2 UV;
in vec3 Tangent;

uniform vec2 viewportSize;
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
#define DIRECTIONAL_LIGHT_MAX_NUM 2
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
uniform sampler2D ssao;

// ******** MATERIAL DATA ********
struct Material {
  bool lit;
  vec3 albedoColor;     
  vec3 emissive;
  float roughness;
  float metallic;

  bool albedoTextureLoaded;
  sampler2D albedoTexture;

  bool roughnessTextureLoaded;
  sampler2D roughnessTexture;

  float normalMapStrength;
  bool normalTextureLoaded;
  sampler2D normalTexture;

  bool metallicTextureLoaded;
  sampler2D metallicTexture;

  bool emissiveTextureLoaded;
  sampler2D emissiveTexture;
};
uniform Material material;

// ******** FUNCTIONS ********
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

float getAmbientOcclusion()
{
  vec2 texCoords = gl_FragCoord.xy / viewportSize;
  return texture(ssao, texCoords).r;
}

vec3 getAlbedo()
{
  if(material.albedoTextureLoaded)
  {
    return material.albedoColor * texture(material.albedoTexture, UV).rgb;
  }
  else
  {
    return material.albedoColor;
  }
}

vec3 calculateAmbient() {
  return ambientLight * getAlbedo();
}

vec3 getEmissive()
{
  return material.emissiveTextureLoaded ? material.emissive * texture(material.emissiveTexture, UV).rgb : material.emissive;
}

float getRoughness()
{
  if(material.roughnessTextureLoaded)
  {
    return material.roughness * texture(material.albedoTexture, UV).r;
  }
  else
  {
    return material.roughness;
  }
}

float getMetallic() 
{
  return material.metallicTextureLoaded ? material.metallic * texture(material.metallicTexture, UV).r : material.metallic;
}

// ******** COOK-TORRANCE BRDF FUNCTIONS ********
float distributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float num = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = 3.14159265359 * denom * denom;
  
  return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;
  
  return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx1 = geometrySchlickGGX(NdotV, roughness);
  float ggx2 = geometrySchlickGGX(NdotL, roughness);
  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 calculateCookTorrance(vec3 L, vec3 lightColor, vec3 normal) {
  float metallic = getMetallic();

  vec3 V = normalize(viewPos - FragPos);
  vec3 H = normalize(V + L);
  float NdotL = max(dot(normal, L), 0.0);
  float NdotV = max(dot(normal, V), 0.0);

  vec3 F0 = mix(vec3(0.04), getAlbedo(), metallic);
  vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
  float roughness = getRoughness();
  float D = distributionGGX(normal, H, roughness);
  float G = GeometrySmith(normal, V, L, roughness);

  vec3 numerator = D * G * F;
  float denominator = 4.0 * NdotV * NdotL + 0.0001;
  vec3 specular = numerator / denominator;

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;

  return (kD * getAlbedo() / 3.14159265359 + specular) * lightColor * NdotL;
}

float calculateSpotFalloff(vec3 lightDir, vec3 spotlightDir, float innerCutoffCosine, float outerCutoffCosine) {
  float theta = dot(lightDir, normalize(-spotlightDir));
  float epsilon = innerCutoffCosine - outerCutoffCosine;
  return clamp((theta - outerCutoffCosine) / epsilon, 0.0, 1.0);
}

vec3 calculateSpotLights(vec3 nrm) {
  vec3 result = vec3(0.0);

  for (int i = 0; i < numSpotLights; ++i) {
    vec3 lightToFrag = FragPos - spotLights[i].position;
    float distanceToLight = length(lightToFrag);

    float attenuation = 1.0 - (distanceToLight / spotLights[i].range);
    attenuation = attenuation * attenuation;

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

        vec3 color = calculateCookTorrance(lightDir, spotLights[i].color, nrm);
        result += attenuation * falloff * color * shadow;
      }
    }
  }

  return result;
}

vec3 calculatePointLights(vec3 nrm) {
  vec3 result = vec3(0.0);

  for (int i = 0; i < numPointLights; ++i) {
    vec3 lightToFrag = FragPos - pointLights[i].position;
    float distanceToLight = length(lightToFrag);
    float relativeIntensity = 1.0 - (distanceToLight / pointLights[i].range);

    if (relativeIntensity > 0.0) {
      relativeIntensity *= relativeIntensity;
      
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
      
      vec3 color = calculateCookTorrance(lightDir, pointLights[i].color, nrm);
      result += shadow * relativeIntensity * color;
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

vec3 calculateDirectionalLights(vec3 nrm) {
  vec3 result = vec3(0.0);
  for (int i = 0; i < numDirectionalLights; ++i) {
    vec3 lightDir = normalize(directionalLights[i].direction);

    // Calculate shadow factor
    int cascadeIndex = chooseLightCascade(directionalLights[i].lightSpaceMatrix, FragPos);
    float shadow = 1.0;
    if (cascadeIndex != -1) {
      shadow = calculateShadowFactor(i, cascadeIndex);
    }

    vec3 color = calculateCookTorrance(lightDir, directionalLights[i].color, nrm);
    result += color * shadow;
  }
  return result;
}

vec3 calculateNormal() {
  vec3 nrm = normalize(Normal);
  if(material.normalTextureLoaded) {
    vec3 T = normalize(Tangent - dot(Tangent, nrm) * nrm);
    vec3 B = cross(nrm, T);
    mat3 TBN = mat3(T, B, nrm);
    
    vec3 sampledNormal = texture(material.normalTexture, UV).rgb * 2.0 - 1.0;
    sampledNormal.xy *= -1;
    vec3 perturbedNormal = normalize(TBN * sampledNormal);
    
    nrm = normalize(mix(nrm, perturbedNormal, material.normalMapStrength));
  }
  return nrm;
}

void main() {
  // Lighting
  vec3 color;
  if(material.lit)
  {
    vec3 nrm = calculateNormal();
    color = calculateAmbient() * getAmbientOcclusion();
    color += calculatePointLights(nrm);
    color += calculateDirectionalLights(nrm);
    color += calculateSpotLights(nrm);
    color += getEmissive();
  }
  else
  {
    color = getAlbedo();
  }

  float transparency = material.albedoTextureLoaded ? texture(material.albedoTexture, UV).a : 1.0;
  FragColor = vec4(color, transparency);
}