#pragma once

#include <glad/glad.h>
#include <AssetManager/Asset.hpp>
#include <vector>
#include <string>

namespace shkyera {

class Mesh : public Asset {
public:
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
    void uploadToGPU(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    GLuint _vao, _vbo, _ebo;
    GLsizei _meshSize;
};

} // namespace shkyera
