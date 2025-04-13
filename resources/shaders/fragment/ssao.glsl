#version 330 core
in vec2 TexCoords;

uniform sampler2D depthTexture;    // View-space depth texture
uniform sampler2D normalTexture;   // View-space normal texture

uniform mat4 projection;

#define NUM_SAMPLES 32
uniform vec3 samples[NUM_SAMPLES];          // SSAO Kernel
uniform float strength;

out float SSAO;

void main() {
    vec3 fragViewPos = texture(depthTexture, TexCoords).xyz; // View-space position
    float fragViewDepth = -fragViewPos.z;    
    vec3 normal = normalize(texture(normalTexture, TexCoords).xyz);

    float occlusion = 0.0;
    float radius = 0.5;  // Sample radius
    float bias = 0.01;

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        vec3 randomVec = vec3(samples[i].y, samples[i].z, samples[i].x);
        vec3 tangent = normalize(normalize(randomVec) - normal * dot(randomVec, normal));
        vec3 bitangent = cross(normal, tangent);
        mat3 TBN = mat3(tangent, bitangent, normal); // Create tangent-space matrix

        vec3 samplePos = TBN * samples[i]; // Transform to view space
        samplePos = fragViewPos + samplePos * radius; // Move sample position

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float offsetDepth = -samplePos.z;
        float newlySampledDepth = -texture(depthTexture, offset.xy).z;

        if(newlySampledDepth + bias < offsetDepth)
        {
            float rangeCheck = smoothstep(0.0, 1.0, radius / abs(newlySampledDepth - fragViewDepth));
            occlusion += rangeCheck;
        }
    }

    occlusion = 1.0 - (occlusion / NUM_SAMPLES); // Normalize occlusion value
    SSAO = clamp(pow(occlusion, strength), 0, 1);
}
