#include <iostream>
#include <fstream>
#include <sstream>

#include <AssetManager/Mesh.hpp>

// Assuming you are using a simple OBJ loader. You could integrate a library like Assimp for more advanced formats.
#include <tiny_obj_loader.h>

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

// Load the mesh from a file (OBJ format for this example)
void Mesh::loadFromFile(const std::string& filepath) {
    // Use a third-party library to load the model, for example TinyOBJLoader
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

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (index.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }

            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            indices.push_back(indices.size());
        }
    }

    uploadToGPU(vertices, indices);
}

void Mesh::uploadToGPU(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    _meshSize = static_cast<GLsizei>(indices.size());

    // Create VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create VBO (vertex buffer object)
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Create EBO (element buffer object)
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    // Assuming the layout is: 3 floats for position, 3 floats for normal, 2 floats for UV
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // UV
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);
}

}
