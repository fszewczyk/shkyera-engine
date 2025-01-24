#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sceneTexture;

const float FXAA_SPAN_MAX = 2.0;
const float FXAA_REDUCE_MIN = 1.0 / 1024.0;

void main() {
    vec3 color = texture(sceneTexture, TexCoords).rgb;

    vec2 texelSize = 1.0 / textureSize(sceneTexture, 0);

    float luma = dot(color, vec3(0.299, 0.587, 0.114));
    float lumaLeft = dot(texture(sceneTexture, TexCoords + vec2(-texelSize.x, 0.0)).rgb, vec3(0.299, 0.587, 0.114));
    float lumaRight = dot(texture(sceneTexture, TexCoords + vec2(texelSize.x, 0.0)).rgb, vec3(0.299, 0.587, 0.114));
    float lumaTop = dot(texture(sceneTexture, TexCoords + vec2(0.0, -texelSize.y)).rgb, vec3(0.299, 0.587, 0.114));
    float lumaBottom = dot(texture(sceneTexture, TexCoords + vec2(0.0, texelSize.y)).rgb, vec3(0.299, 0.587, 0.114));

    float lumaMin = min(luma, min(min(lumaLeft, lumaRight), min(lumaTop, lumaBottom)));
    float lumaMax = max(luma, max(max(lumaLeft, lumaRight), max(lumaTop, lumaBottom)));

    vec2 dir = vec2(
        -((lumaLeft + lumaRight) - (lumaTop + lumaBottom)),
        ((lumaLeft + lumaRight) + (lumaTop + lumaBottom))
    );

    float dirReduce = max(
        (lumaMax - lumaMin) / (lumaMax + lumaMin),
        FXAA_REDUCE_MIN
    );
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = clamp(dir * rcpDirMin, -FXAA_SPAN_MAX, FXAA_SPAN_MAX) * texelSize;

    // Sample along the edge
    vec3 result = 0.5 * (
        texture(sceneTexture, TexCoords + dir * (1.0 / 3.0)).rgb +
        texture(sceneTexture, TexCoords + dir * (2.0 / 3.0)).rgb
    );

    FragColor = vec4(result, 1.0);
}
