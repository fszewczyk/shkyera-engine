#version 330 core

in vec3 Normal;

out vec3 NormalOut;

void main() {
  NormalOut = normalize(Normal);
}
