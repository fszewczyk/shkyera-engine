#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <AssetManager/Shader.hpp>

namespace shkyera {

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    // Attach a shader to the program
    void attachShader(const std::shared_ptr<Shader>& shader);

    // Link all attached shaders to create the shader program
    void link();

    // Use the shader program (glUseProgram)
    void use() const;

private:
    GLuint _id;   // OpenGL shader program ID

    GLint getUniformLocation(const std::string& name);
};

}