#include <Common/Logger.hpp>
#include <Rendering/ShaderProgram.hpp>

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
        Logger::ERROR("Shader Linking Failed: " + std::string(infoLog));
    }
}

bool ShaderProgram::isInUse() {
    return !_shadersInUse.empty() && _id == _shadersInUse.top();
}

void ShaderProgram::use() {
    _shadersInUse.push(_id);
    glUseProgram(_id);
}

void ShaderProgram::stopUsing() {
    _shadersInUse.pop();
    glUseProgram(_shadersInUse.empty() ? 0 : _shadersInUse.top());
}

void ShaderProgram::setUniform(const std::string& name, int value) {
    if (!isInUse()) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) {  // Only set uniform if the location is valid
        glUniform1i(location, value);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, float value) {
    if (!isInUse()) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) {  // Only set uniform if the location is valid
        glUniform1f(location, value);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) {
    if (!isInUse()) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) {  // Only set uniform if the location is valid
        glUniform2fv(location, 1, &value[0]);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) {
    if (!isInUse()) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) {  // Only set uniform if the location is valid
        glUniform3fv(location, 1, &value[0]);
    } else {
        Logger::ERROR("Shader uniform cannot be set due to invalid location '" + name);
    }
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) {
    if (!isInUse()) {
        Logger::ERROR("Trying to set a uniform (" + name + ") while the shader program is not in use.");
    }

    GLint location = getUniformLocation(name);
    if (location != -1) {  // Only set uniform if the location is valid
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

UseShader::UseShader(ShaderProgram& shaderProgram)
    : mShaderProgram(shaderProgram) {
    mShaderProgram.use();
}

UseShader::~UseShader() {
    mShaderProgram.stopUsing();
}

}  // namespace shkyera
