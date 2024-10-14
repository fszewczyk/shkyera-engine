#include <Rendering/ShaderProgram.hpp>

#include <iostream>

namespace shkyera {

ShaderProgram::ShaderProgram() {
    _id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(_id);
}

void ShaderProgram::attachShader(const std::shared_ptr<Shader>& shader) {
    glAttachShader(_id, shader->getID());
}

void ShaderProgram::link() {
    glLinkProgram(_id);

    // Check for linking errors
    GLint success;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(_id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void ShaderProgram::use() const {
    glUseProgram(_id);
}

void ShaderProgram::setUniform(const std::string& name, float value) {
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) {
    GLint location = getUniformLocation(name);
    glUniform3fv(location, 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

GLint ShaderProgram::getUniformLocation(const std::string& name) {
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end()) {
        return _uniformLocationCache[name];
    }

    GLint location = glGetUniformLocation(_id, name.c_str());

    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or wasn't used." << std::endl;
    }

    _uniformLocationCache[name] = location;
    return location;
}

}
