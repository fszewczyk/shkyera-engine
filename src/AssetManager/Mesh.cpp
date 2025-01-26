#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <numeric>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <tiny_obj_loader.h>

#include <AssetManager/Mesh.hpp>
#include <Common/Logger.hpp>

namespace std {
    template <>
    struct std::hash<glm::vec3> {
        size_t operator()(const glm::vec3& vec) const {
            return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y) ^ std::hash<float>()(vec.z);
        }
    };
}

namespace shkyera {

Mesh::Mesh(const std::filesystem::path& path) : PathConstructibleAsset(path) {
    loadFromFile(path);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) : _vertices(std::move(vertices)), _indices(std::move(indices)) {
    uploadToGPU();
}

Mesh::Mesh(Mesh&& other) noexcept
    : PathConstructibleAsset(std::move(other)),
      _vertices(std::move(other._vertices)),
      _indices(std::move(other._indices)),
      _vao(std::exchange(other._vao, 0)),
      _vbo(std::exchange(other._vbo, 0)),
      _ebo(std::exchange(other._ebo, 0)),
      _meshSize(std::exchange(other._meshSize, 0))
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept 
{
    if (this != &other) 
    {
        PathConstructibleAsset::operator=(std::move(other));

        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);

        _vertices = std::move(other._vertices);
        _indices = std::move(other._indices);
        _vao = std::exchange(other._vao, 0);
        _vbo = std::exchange(other._vbo, 0);
        _ebo = std::exchange(other._ebo, 0);
        _meshSize = std::exchange(other._meshSize, 0);
    }
    return *this;
}

void Mesh::draw() const {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, getMeshSize());
    unbind();
}

// Destructor to clean up OpenGL resources
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

AABB Mesh::getBoundingBox() const {
    if (_vertices.empty()) {
        return shkyera::AABB();
    }

    glm::vec3 minBounds = {FLT_MAX, FLT_MAX, FLT_MAX};
    glm::vec3 maxBounds = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

    for (const auto& vertex : _vertices) {
        minBounds = glm::min(minBounds, vertex.position);
        maxBounds = glm::max(maxBounds, vertex.position);
    }

    glm::vec3 center = (minBounds + maxBounds) * 0.5f;
    glm::vec3 extents = (maxBounds - minBounds) * 0.5f;

    Logger::INFO(glm::to_string(center) + " " + glm::to_string(extents));

    return shkyera::AABB{.center = center, .extents = extents};
}

static std::vector<glm::vec3> calculateNormals(const std::vector<Mesh::Vertex>& vertices, const std::vector<uint32_t>& indices) {
    std::unordered_map<glm::vec3, glm::vec3> vertexToNormalMap;
    std::unordered_map<glm::vec3, std::vector<glm::vec3>> faceNormals;

    for (size_t faceIndex = 0; faceIndex < indices.size(); faceIndex += 3) {
        unsigned int idx0 = indices[faceIndex];
        unsigned int idx1 = indices[faceIndex + 1];
        unsigned int idx2 = indices[faceIndex + 2];

        glm::vec3 v0 = vertices[idx0].position;
        glm::vec3 v1 = vertices[idx1].position;
        glm::vec3 v2 = vertices[idx2].position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        faceNormals[v0].push_back(faceNormal);
        faceNormals[v1].push_back(faceNormal);
        faceNormals[v2].push_back(faceNormal);
    }

    std::vector<glm::vec3> vertexToNormal(vertices.size(), glm::vec3(0.0f));

    for (const auto& entry : faceNormals) {
        const glm::vec3& position = entry.first;
        const std::vector<glm::vec3>& normals = entry.second;

        glm::vec3 averagedNormal = glm::vec3(0.0f);
        for (const glm::vec3& normal : normals) {
            averagedNormal += normal;
        }
        averagedNormal = glm::normalize(averagedNormal);

        vertexToNormalMap[position] = averagedNormal;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexToNormal[i] = vertexToNormalMap[vertices[i].position];
    }

    return vertexToNormal;
}

void Mesh::loadFromFile(const std::filesystem::path& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        Logger::ERROR(std::string("Failed to load OBJ file: ") + filepath.c_str());
        return;
    }

    _vertices.clear();
    _indices.clear();

    size_t numberOfVertices = std::accumulate(shapes.begin(), shapes.end(), 0, [](size_t accumulatedSize, const auto& shape) { return accumulatedSize + shape.mesh.indices.size(); });
    _vertices.reserve(numberOfVertices);
    _indices.reserve(numberOfVertices);

    bool hasNormals = false;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 position(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            );

            glm::vec3 normal(0.0f, 0.0f, 0.0f);
            if (index.normal_index >= 0) {
                normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );
                hasNormals = true;
            }

            glm::vec2 texcoord(0.0f, 0.0f);
            if (index.texcoord_index >= 0) {
                texcoord = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                );
            }

            _vertices.emplace_back(position, normal, texcoord);
            _indices.emplace_back(_indices.size());
        }
    }

    if (!hasNormals) {
        auto calculatedNormals = calculateNormals(_vertices, _indices);

        for (size_t i = 0; i < _vertices.size(); ++i) {
            _vertices[i].normal = calculatedNormals[i];
        }
    }

    uploadToGPU();
}

void Mesh::uploadToGPU() {
    _meshSize = static_cast<GLsizei>(_indices.size());

    // Create VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create VBO (vertex buffer object)
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // Create EBO (element buffer object)
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));   // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));  // UV
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);
}


Mesh Mesh::Factory::createPlane() {
    std::vector<Vertex> vertices = {
        { { -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },  // 0
        { {  1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },  // 1
        { {  1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },  // 2
        { { -1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }   // 3
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createCube() {
      std::vector<Vertex> vertices = {
        // Front face
        { { -1.0f, -1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },  // 0
        { {  1.0f, -1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },  // 1
        { {  1.0f,  1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },  // 2
        { { -1.0f,  1.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },  // 3

        // Back face
        { { -1.0f, -1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },  // 4
        { {  1.0f, -1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },  // 5
        { {  1.0f,  1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } },  // 6
        { { -1.0f,  1.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },  // 7

        // Left face
        { { -1.0f, -1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } }, // 8
        { { -1.0f,  1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } }, // 9
        { { -1.0f,  1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } }, // 10
        { { -1.0f, -1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } }, // 11

        // Right face
        { {  1.0f, -1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } }, // 12
        { {  1.0f,  1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } }, // 13
        { {  1.0f,  1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } }, // 14
        { {  1.0f, -1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } }, // 15

        // Top face
        { { -1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f } }, // 16
        { {  1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } }, // 17
        { {  1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f } }, // 18
        { { -1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } }, // 19

        // Bottom face
        { { -1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } }, // 20
        { {  1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } }, // 21
        { {  1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } }, // 22
        { { -1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } }, // 23
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 11, 10, 10, 9, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 23, 22, 22, 21, 20,
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createCubeMap() {
    std::vector<Mesh::Vertex> vertices = {
        {{-1.0f,  1.0f, -1.0f}},
        {{-1.0f, -1.0f, -1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{1.0f,  1.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{-1.0f, -1.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}},
        {{-1.0f,  1.0f,  1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{1.0f, -1.0f,  1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{1.0f,  1.0f, -1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{1.0f, -1.0f,  1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{-1.0f,  1.0f, -1.0f}},
        {{1.0f,  1.0f, -1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{1.0f,  1.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}},
        {{-1.0f,  1.0f, -1.0f}},
        {{-1.0f, -1.0f, -1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{1.0f, -1.0f, -1.0f}},
        {{-1.0f, -1.0f,  1.0f}},
        {{1.0f, -1.0f,  1.0f}}
    };

    std::vector<unsigned int> indices(36);
    std::iota(indices.begin(), indices.end(), 0);

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createCylinder() {
    constexpr int sectors = 36;
    constexpr float radius = 1.0f;
    constexpr float height = 1.0f;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int i = 0; i <= sectors; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(sectors);
        float x = radius * cos(theta);
        float z = radius * sin(theta);

        vertices.push_back({ { x, -height / 2, z }, { x, 0.0f, z }, { float(i) / sectors, 0.0f } });
        vertices.push_back({ { x,  height / 2, z }, { x, 0.0f, z }, { float(i) / sectors, 1.0f } });
    }

    // Generate indices for the sides
    for (int i = 0; i < sectors; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i * 2 + 2) % (sectors * 2));

        indices.push_back(i * 2 + 1);
        indices.push_back((i * 2 + 3) % (sectors * 2));
        indices.push_back((i * 2 + 2) % (sectors * 2));
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createCone() {
    constexpr int sectors = 36;
    constexpr float radius = 1.0f;
    constexpr float height = 2.0f;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int i = 0; i <= sectors; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(sectors);
        float x = radius * cos(theta);
        float z = radius * sin(theta);

        vertices.push_back({ { x, -height / 2, z }, { x, 0.0f, z }, { float(i) / sectors, 0.0f } });
        vertices.push_back({ { 0,  height / 2, 0 }, { 0, 0.0f, 1 }, { float(i) / sectors, 1.0f } });
    }

    // Generate indices for the sides
    for (int i = 0; i < sectors; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i * 2 + 2) % (sectors * 2));

        indices.push_back(i * 2 + 1);
        indices.push_back((i * 2 + 3) % (sectors * 2));
        indices.push_back((i * 2 + 2) % (sectors * 2));
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createTorus(float innerRadius, float outerRadius, int radialSegments, int tubularSegments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;
    float tubeRadius = (outerRadius - innerRadius) * 0.5f;
    float torusRadius = innerRadius + tubeRadius;

    for (int i = 0; i <= radialSegments; ++i) {
        float u = static_cast<float>(i) / radialSegments;
        float theta = u * 2.0f * PI;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);

        for (int j = 0; j <= tubularSegments; ++j) {
            float v = static_cast<float>(j) / tubularSegments;
            float phi = v * 2.0f * PI;
            float cosPhi = std::cos(phi);
            float sinPhi = std::sin(phi);

            glm::vec3 center(torusRadius * cosTheta, torusRadius * sinTheta, 0.0f);
            glm::vec3 position = center + glm::vec3(tubeRadius * cosTheta * cosPhi, tubeRadius * sinTheta * cosPhi, tubeRadius * sinPhi);
            glm::vec3 normal = glm::normalize(glm::vec3(cosTheta * cosPhi, sinTheta * cosPhi, sinPhi));
            glm::vec2 texcoord(u, v);

            vertices.push_back({ position, normal, texcoord });
        }
    }

    // Generate indices
    for (int i = 0; i < radialSegments; ++i) {
        for (int j = 0; j < tubularSegments; ++j) {
            int a = i * (tubularSegments + 1) + j;
            int b = (i + 1) * (tubularSegments + 1) + j;
            int c = (i + 1) * (tubularSegments + 1) + (j + 1);
            int d = i * (tubularSegments + 1) + (j + 1);

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    return Mesh(vertices, indices);
}

Mesh Mesh::Factory::createSphere() {
    const int stacks = 24;
    const int sectors = 48;
    const float radius = 1.0f;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2.0f - i * (M_PI / stacks);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * (2.0f * M_PI / sectors);
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertices.push_back({ { x, y, z }, { x / radius, y / radius, z / radius }, { float(j) / sectors, float(i) / stacks } });
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            unsigned int first = (i * (sectors + 1)) + j;
            unsigned int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return Mesh(vertices, indices);
}

}
