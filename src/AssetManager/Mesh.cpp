#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>

#include <AssetManager/Mesh.hpp>
// Assuming you are using a simple OBJ loader. You could integrate a library like Assimp for more advanced formats.
#include <tiny_obj_loader.h>

namespace std {
    template <>
    struct std::hash<glm::vec3> {
        size_t operator()(const glm::vec3& vec) const {
            return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y) ^ std::hash<float>()(vec.z);
        }
    };
}

namespace shkyera {

// Constructor that loads the model from a file
Mesh::Mesh(const std::string& filepath) {
    loadFromFile(filepath);
}

// Destructor to clean up OpenGL resources
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}


static std::vector<glm::vec3> calculateNormals(const std::vector<Mesh::Vertex>& vertices, const std::vector<unsigned int>& indices) {
    std::unordered_map<glm::vec3, glm::vec3> vertexToNormalMap;
    std::unordered_map<glm::vec3, std::vector<glm::vec3>> faceNormals;

    for (size_t faceIndex = 0; faceIndex < indices.size(); faceIndex += 3) {
        unsigned int idx0 = indices[faceIndex];
        unsigned int idx1 = indices[faceIndex + 1];
        unsigned int idx2 = indices[faceIndex + 2];

        glm::vec3 v0 = vertices[idx0].position;
        glm::vec3 v1 = vertices[idx1].position;
        glm::vec3 v2 = vertices[idx2].position;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        faceNormals[v0].push_back(faceNormal);
        faceNormals[v1].push_back(faceNormal);
        faceNormals[v2].push_back(faceNormal);
    }

    std::vector<glm::vec3> vertexToNormal(vertices.size(), glm::vec3(0.0f));

    for (const auto& entry : faceNormals) {
        const glm::vec3& position = entry.first;
        const std::vector<glm::vec3>& normals = entry.second;

        // Average the face normals for each position
        glm::vec3 averagedNormal = glm::vec3(0.0f);
        for (const glm::vec3& normal : normals) {
            averagedNormal += normal;
        }
        averagedNormal = glm::normalize(averagedNormal);

        // Map the averaged normal back to the vertices
        vertexToNormalMap[position] = averagedNormal;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexToNormal[i] = vertexToNormalMap[vertices[i].position];
    }

    return vertexToNormal;
}

void Mesh::loadFromFile(const std::string& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        std::cerr << "Failed to load OBJ file: " << filepath << std::endl;
        if (!warn.empty()) std::cerr << "Warning: " << warn << std::endl;
        if (!err.empty()) std::cerr << "Error: " << err << std::endl;
        return;
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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

            vertices.emplace_back(position, normal, texcoord);
            indices.push_back(indices.size());
        }
    }

    if (!hasNormals) {
        auto calculatedNormals = calculateNormals(vertices, indices);

        for (size_t i = 0; i < vertices.size(); ++i) {
            vertices[i].normal = calculatedNormals[i];
        }
    }

    uploadToGPU(vertices, indices);
}

void Mesh::uploadToGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    _meshSize = static_cast<GLsizei>(indices.size());

    // Create VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create VBO (vertex buffer object)
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Create EBO (element buffer object)
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

}
