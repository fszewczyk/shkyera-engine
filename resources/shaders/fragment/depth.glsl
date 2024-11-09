#version 330 core

in float FragDepth;

out float FragValue;

void main() {
  FragValue = FragDepth;
}
