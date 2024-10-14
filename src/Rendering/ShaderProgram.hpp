#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include <AssetManager/Shader.hpp>

namespace shkyera {

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void attachShader(const std::shared_ptr<Shader>& shader);

    void link();

    void use() const;

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::mat4& value);

private:
    GLuint _id;   // OpenGL shader program ID
    std::unordered_map<std::string, GLint> _uniformLocationCache;

    GLint getUniformLocation(const std::string& name);
};

}