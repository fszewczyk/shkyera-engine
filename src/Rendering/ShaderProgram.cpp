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

}
