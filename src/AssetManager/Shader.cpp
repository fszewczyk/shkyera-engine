#include <fstream>
#include <sstream>

#include <iostream>
#include <AssetManager/Shader.hpp>

namespace shkyera {

Shader::Shader(const std::string& filepath, Type type) : _type(type) {
    std::string source = loadFromFile(filepath);
    compile(source);
}

Shader::~Shader() {
    glDeleteShader(_id);
}

std::string Shader::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::compile(const std::string& source) {
    const char* src = source.c_str();
    GLenum glShaderType = shaderTypeToGLenum(_type);

    _id = glCreateShader(glShaderType);
    glShaderSource(_id, 1, &src, nullptr);
    glCompileShader(_id);

    GLint success;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(_id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

GLenum Shader::shaderTypeToGLenum(Type type) {
    switch (type) {
        case Type::Vertex:
            return GL_VERTEX_SHADER;
        case Type::Geometry:
            return GL_GEOMETRY_SHADER;
        case Type::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            return 0;
    }
}
    
}