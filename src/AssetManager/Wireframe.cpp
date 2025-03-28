#include <AssetManager/Wireframe.hpp>
#include <Common/Logger.hpp>
#include <tiny_obj_loader.h>
#include <iostream>

namespace shkyera {

Wireframe::Wireframe(const std::filesystem::path& filepath) : PathConstructibleAsset(filepath) {
    loadFromFile(filepath);
}

Wireframe::Wireframe(const std::vector<Edge>& edges) {
    uploadToGPU(edges);
}

Wireframe::Wireframe(Wireframe&& other) noexcept
    : PathConstructibleAsset(std::move(other)),
      _vao(std::exchange(other._vao, 0)),
      _vbo(std::exchange(other._vbo, 0)),
      _edgeCount(std::exchange(other._edgeCount, 0)) {}

Wireframe& Wireframe::operator=(Wireframe&& other) noexcept {
    if (this != &other) {
        PathConstructibleAsset::operator=(std::move(other));

        if(_vao != 0)
        {
            glDeleteVertexArrays(1, &_vao);
        }
        if(_vbo != 0)
        {
            glDeleteBuffers(1, &_vbo);
        }

        _vao = std::exchange(other._vao, 0);
        _vbo = std::exchange(other._vbo, 0);
        _edgeCount = std::exchange(other._edgeCount, 0);
    }
    return *this;
}

Wireframe::~Wireframe() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void Wireframe::loadFromFile(const std::filesystem::path& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        Logger::ERROR(std::string("Failed to load OBJ file: ") + filepath.c_str());
        return;
    }

    std::vector<Edge> edges;

    for (const auto& shape : shapes) {
        const auto& mesh = shape.mesh;
        for (size_t faceIdx = 0; faceIdx < mesh.indices.size(); faceIdx += 3) {
            glm::vec3 positions[3];
            for (int i = 0; i < 3; ++i) {
                unsigned int idx = mesh.indices[faceIdx + i].vertex_index;
                positions[i] = glm::vec3(
                    attrib.vertices[3 * idx + 0],
                    attrib.vertices[3 * idx + 1],
                    attrib.vertices[3 * idx + 2]
                );
            }

            edges.emplace_back(positions[0], positions[1]);
            edges.emplace_back(positions[1], positions[2]);
            edges.emplace_back(positions[2], positions[0]);
        }
    }

    uploadToGPU(edges);
}

void Wireframe::uploadToGPU(const std::vector<Edge>& edges) {
    _edgeCount = static_cast<GLsizei>(edges.size() * 2); // Two vertices per edge

    std::vector<glm::vec3> edgeVertices;
    edgeVertices.reserve(_edgeCount);
    for (const auto& edge : edges) {
        edgeVertices.push_back(edge.start);
        edgeVertices.push_back(edge.end);
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, edgeVertices.size() * sizeof(glm::vec3), edgeVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Wireframe Wireframe::Factory::createCube() {
    std::vector<Edge> edges = {
        // Front face
        { { -1.0f, -1.0f, -1.0f }, {  1.0f, -1.0f, -1.0f } },
        { {  1.0f, -1.0f, -1.0f }, {  1.0f,  1.0f, -1.0f } },
        { {  1.0f,  1.0f, -1.0f }, { -1.0f,  1.0f, -1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f } },

        // Back face
        { { -1.0f, -1.0f,  1.0f }, {  1.0f, -1.0f,  1.0f } },
        { {  1.0f, -1.0f,  1.0f }, {  1.0f,  1.0f,  1.0f } },
        { {  1.0f,  1.0f,  1.0f }, { -1.0f,  1.0f,  1.0f } },
        { { -1.0f,  1.0f,  1.0f }, { -1.0f, -1.0f,  1.0f } },

        // Connect front and back faces
        { { -1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f,  1.0f } },
        { {  1.0f, -1.0f, -1.0f }, {  1.0f, -1.0f,  1.0f } },
        { {  1.0f,  1.0f, -1.0f }, {  1.0f,  1.0f,  1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { -1.0f,  1.0f,  1.0f } },
    };

    return Wireframe(edges);
}

Wireframe Wireframe::Factory::createCylinder() {
    const int sectors = 36;
    const float radius = 1.0f;
    const float height = 2.0f;

    std::vector<Edge> edges;

    // Generate vertices for the top and bottom circles
    for (int i = 0; i < sectors; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(sectors);
        float nextTheta = 2.0f * M_PI * float(i + 1) / float(sectors);

        float x = radius * cos(theta);
        float z = radius * sin(theta);
        float nextX = radius * cos(nextTheta);
        float nextZ = radius * sin(nextTheta);

        glm::vec3 bottomCurrent = { x, -height / 2, z };
        glm::vec3 bottomNext = { nextX, -height / 2, nextZ };
        glm::vec3 topCurrent = { x, height / 2, z };
        glm::vec3 topNext = { nextX, height / 2, nextZ };

        // Edges on bottom and top circles
        edges.emplace_back(bottomCurrent, bottomNext);
        edges.emplace_back(topCurrent, topNext);

        // Vertical edges connecting top and bottom
        edges.emplace_back(bottomCurrent, topCurrent);
    }

    return Wireframe(edges);
}

Wireframe Wireframe::Factory::createSphere() {
    const int stacks = 18;
    const int sectors = 36;
    const float radius = 1.0f;

    std::vector<Edge> edges;

    for (int i = 0; i < stacks; ++i) {
        float stackAngle = M_PI / 2.0f - i * (M_PI / stacks);
        float nextStackAngle = M_PI / 2.0f - (i + 1) * (M_PI / stacks);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        float nextXY = radius * cosf(nextStackAngle);
        float nextZ = radius * sinf(nextStackAngle);

        for (int j = 0; j < sectors; ++j) {
            float sectorAngle = 2.0f * M_PI * j / sectors;
            float nextSectorAngle = 2.0f * M_PI * (j + 1) / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float nextX = xy * cosf(nextSectorAngle);
            float nextY = xy * sinf(nextSectorAngle);

            glm::vec3 current = { x, y, z };
            glm::vec3 next = { nextX, nextY, z };
            glm::vec3 nextStackCurrent = { x * nextXY / xy, y * nextXY / xy, nextZ };

            // Horizontal edge
            edges.emplace_back(current, next);

            // Vertical edge to the next stack
            edges.emplace_back(current, nextStackCurrent);
        }
    }

    return Wireframe(edges);
}

} // namespace shkyera
