#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>

#include <AssetManager/PathConstructibleAsset.hpp>

namespace shkyera {

class Wireframe : public PathConstructibleAsset<Wireframe> {
public:
    struct Edge {
        glm::vec3 start;
        glm::vec3 end;

        Edge(const glm::vec3& s, const glm::vec3& e) : start(s), end(e) {}
    };

    Wireframe(const std::filesystem::path& filepath);
    Wireframe(const std::vector<Edge>& edges);

    Wireframe(const Wireframe& other) = delete;
    Wireframe& operator=(const Wireframe& other) = delete;

    Wireframe(Wireframe&& other) noexcept;
    Wireframe& operator=(Wireframe&& other) noexcept;

    ~Wireframe();

    void bind() const { glBindVertexArray(_vao); }
    void unbind() const { glBindVertexArray(0); }

    GLuint getVAO() const { return _vao; }
    GLuint getVBO() const { return _vbo; }
    GLsizei getEdgeCount() const { return _edgeCount; }

    class Factory {
    public:
        static Wireframe createCube();
        static Wireframe createCylinder();
        static Wireframe createSphere();
    };

private:
    void loadFromFile(const std::filesystem::path& filepath);
    void uploadToGPU(const std::vector<Edge>& edges);

    GLuint _vao, _vbo;
    GLsizei _edgeCount;
};

} // namespace shkyera
