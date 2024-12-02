#version 330 core

in vec3 FragPos;

out float FragValue;

void main() {
  FragValue = length(FragPos);// / 30.0;
}
