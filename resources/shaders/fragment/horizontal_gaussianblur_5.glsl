#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D textureToBlur;
uniform vec2 resolution;

void main() {
    vec2 texelSize = 1.0 / resolution;
    vec3 result = vec3(0.0);
    float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    for (int i = -4; i <= 4; ++i) {
        result += texture(textureToBlur, TexCoords + vec2(texelSize.x * i, 0.0)).rgb * weights[abs(i)];
    }

    FragColor = vec4(result, 1.0);
}
