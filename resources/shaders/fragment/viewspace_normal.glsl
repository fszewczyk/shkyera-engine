#version 330 core
in vec3 Normal;

out vec3 OutNormal;

void main() {
    OutNormal = normalize(Normal);
}
