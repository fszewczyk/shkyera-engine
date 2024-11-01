#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D silhouetteTexture;
uniform int kernelSize = 3;
uniform bool horizontal;

void main() {
  vec2 tex_offset = 1.0 / textureSize(silhouetteTexture, 0);
  vec3 result = texture(silhouetteTexture, TexCoords).rgb;

  if (horizontal) {
    for (int i = 1; i < kernelSize + 4; ++i) {
      result = max(
          texture(silhouetteTexture, TexCoords + vec2(tex_offset.x * i, 0.0))
              .rgb,
          result);
      result = max(
          texture(silhouetteTexture, TexCoords - vec2(tex_offset.x * i, 0.0))
              .rgb,
          result);
    }
  } else {
    for (int i = 1; i < kernelSize; ++i) {
      result = max(
          texture(silhouetteTexture, TexCoords + vec2(0.0, tex_offset.y * i))
              .rgb,
          result);
      result = max(
          texture(silhouetteTexture, TexCoords - vec2(0.0, tex_offset.y * i))
              .rgb,
          result);
    }
  }

  FragColor = vec4(result, 1.0);
}
