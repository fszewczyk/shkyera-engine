#version 330 core

in float VertexDepth;

out float FragValue;

void main() {
  FragValue = VertexDepth;
}
