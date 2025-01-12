#include <Rendering/ShaderProgram.hpp>
#include <Common/Logger.hpp>

namespace shkyera {

ShaderProgram::ShaderProgram() : _inUse(false) {
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
        Logger::ERROR("Shader Linking Failed: " + std::string(infoLog));
    }
}

void ShaderProgram::use() {
    glUseProgram(_id);
    _inUse = true;
}

void ShaderProgram::stopUsing() {
    glUseProgram(0);
    _inUse = false;
}

void ShaderProgram::setUniform(const std::string& name, int value) {
    if(!_inUse) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) { // Only set uniform if the location is valid
        glUniform1i(location, value);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, float value) {
    if(!_inUse) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) { // Only set uniform if the location is valid
        glUniform1f(location, value);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) {
    if(!_inUse) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) { // Only set uniform if the location is valid
        glUniform3fv(location, 1, &value[0]);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) {
    if(!_inUse) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) { // Only set uniform if the location is valid
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

GLint ShaderProgram::getUniformLocation(const std::string& name) {
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end()) {
        return _uniformLocationCache[name];
    }

    GLint location = glGetUniformLocation(_id, name.c_str());

    if (location == -1) {
        Logger::ERROR("Uniform '" + name + "' doesn't exist or wasn't used.");
    }

    _uniformLocationCache[name] = location;
    return location;
}

}
