#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform float threshold; // The threshold value for bloom

void main() {
    vec3 color = texture(sceneTexture, TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // Perceived brightness
    vec3 bloomColor = brightness > threshold ? color : vec3(0.0);
    FragColor = vec4(bloomColor, 1.0);
}