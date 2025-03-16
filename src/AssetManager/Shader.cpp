#include <fstream>
#include <sstream>

#include <AssetManager/Shader.hpp>
#include <Common/Logger.hpp>
#include <iostream>

namespace shkyera {

Shader::Shader(const std::filesystem::path& filepath, Type type)
    : PathConstructibleAsset(filepath),
      _type(type) {
    std::string source = loadFromFile(filepath);
    compile(source);
}

Shader::~Shader() {
    if (_id != 0) {
        glDeleteShader(_id);
    }
}

Shader::Shader(Shader&& other) noexcept
    : PathConstructibleAsset(std::move(other)),
      _id(std::exchange(other._id, 0)),
      _type(other._type) {}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        PathConstructibleAsset::operator=(std::move(other));

        if (_id != 0) {
            glDeleteShader(_id);
        }

        _id = std::exchange(other._id, 0);
        _type = other._type;
    }

    return *this;
}

std::string Shader::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::ERROR("Failed to open shader file: " + filepath);
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
        Logger::ERROR("Shader Compilation Failed: " + std::string(infoLog));
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

}  // namespace shkyera