#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;     // View-space normal

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vec4 viewPos = view * worldPos;
    Normal = mat3(transpose(inverse(view))) * aNormal; // Transform normal to view space
    gl_Position = projection * viewPos;
}
