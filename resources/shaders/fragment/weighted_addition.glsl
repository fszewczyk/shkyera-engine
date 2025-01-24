#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D firstTexture;
uniform sampler2D secondTexture;
uniform float firstWeight;
uniform float secondWeight;

void main() {
    vec3 firstColor = texture(firstTexture, TexCoords).rgb;
    vec3 secondColor = texture(secondTexture, TexCoords).rgb;
    FragColor = vec4((firstColor * firstWeight) + (secondColor * secondWeight), 1.0);
}