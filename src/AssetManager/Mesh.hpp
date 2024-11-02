#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>

#include <AssetManager/Asset.hpp>
#include <Math/Box.hpp>

namespace shkyera {

class Mesh : public Asset {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;

        Vertex(const glm::vec3& pos)
            : position(pos), normal(0), texcoord(0) {}
        Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
            : position(pos), normal(norm), texcoord(tex) {}
    };

    Mesh(const std::string& filepath);
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    ~Mesh();

    void bind() const { glBindVertexArray(_vao); }
    void unbind() const { glBindVertexArray(0); }
    void draw() const;

    Box getBoundingBox() const;

    GLuint getVAO() const { return _vao; }
    GLuint getVBO() const { return _vbo; }
    GLuint getEBO() const { return _ebo; }
    GLsizei getMeshSize() const { return _meshSize; }

    class Factory {
        public:
            static Mesh* createCube();
            static Mesh* createCubeMap();
            static Mesh* createCylinder();
            static Mesh* createSphere();
    };

private:
    void loadFromFile(const std::string& filepath);
    void uploadToGPU();

    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    GLuint _vao, _vbo, _ebo;
    GLsizei _meshSize;
};

} // namespace shkyera
