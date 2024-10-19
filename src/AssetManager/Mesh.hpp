#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <AssetManager/Asset.hpp>
#include <vector>
#include <string>

namespace shkyera {

class Mesh : public Asset {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;

        Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
            : position(pos), normal(norm), texcoord(tex) {}
    };

    Mesh(const std::string& filepath);
    ~Mesh();

    void bind() const { glBindVertexArray(_vao); }
    void unbind() const { glBindVertexArray(0); }

    GLuint getVAO() const { return _vao; }
    GLuint getVBO() const { return _vbo; }
    GLuint getEBO() const { return _ebo; }
    GLsizei getMeshSize() const { return _meshSize; }

    std::vector<float> _vertices;
    std::vector<unsigned int> _indices;

private:
    void loadFromFile(const std::string& filepath);
    void uploadToGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    GLuint _vao, _vbo, _ebo;
    GLsizei _meshSize;
};

} // namespace shkyera
