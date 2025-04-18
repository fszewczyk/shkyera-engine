#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <Math/AABB.hpp>

namespace shkyera {

class Mesh {
 public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texcoord;

    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
        : position(pos), normal(norm), texcoord(tex) {
      calculateTangent();
    }
    Vertex(const glm::vec3& pos) : Vertex(pos, glm::vec3{0, 1, 0}, glm::vec2{0}) {}

    void calculateTangent() {
      if (std::abs(normal.x) > std::abs(normal.z)) {
        tangent = glm::normalize(glm::cross(normal, glm::vec3(0, 0, 1)));
      } else {
        tangent = glm::normalize(glm::cross(normal, glm::vec3(1, 0, 0)));
      }
    }
  };

  Mesh(const std::filesystem::path& path);
  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

  Mesh(const Mesh& other) = delete;
  Mesh& operator=(const Mesh& other) = delete;

  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;

  ~Mesh();

  void bind() const { glBindVertexArray(_vao); }
  void unbind() const { glBindVertexArray(0); }
  void draw() const;

  AABB getBoundingBox() const;

  GLuint getVAO() const { return _vao; }
  GLuint getVBO() const { return _vbo; }
  GLuint getEBO() const { return _ebo; }
  GLsizei getMeshSize() const { return _meshSize; }

  class Factory {
   public:
    enum class Type { UNDEFINED = 0, PLANE = 1, CUBE = 2, CUBEMAP = 3, CYLINDER = 4, CONE = 5, SPHERE = 6 };

    static Mesh create(Type type);
    static Mesh createTorus(float innerRadius, float outerRadius, int radialSegments, int tubularSegments);

   private:
    static Mesh createPlane();
    static Mesh createCube();
    static Mesh createCubeMap();
    static Mesh createCylinder();
    static Mesh createCone();
    static Mesh createSphere();
  };

 private:
  void loadFromFile(const std::filesystem::path& filepath);
  void uploadToGPU();

  std::vector<Vertex> _vertices;
  std::vector<uint32_t> _indices;
  GLuint _vao, _vbo, _ebo;
  GLsizei _meshSize;
};

}  // namespace shkyera
