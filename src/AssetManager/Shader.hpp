#pragma once

#include <glad/glad.h>

#include <AssetManager/Asset.hpp>

namespace shkyera {

class Shader : public Asset {
public:
    enum class Type {
        Vertex,
        Geometry,
        Fragment,
    };

    Shader(const std::string& filepath, Type type);
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    
    ~Shader();

    GLuint getID() const { return _id; }

private:
    GLuint _id;
    Type _type;

    std::string loadFromFile(const std::string& filepath);

    void compile(const std::string& source);

    GLenum shaderTypeToGLenum(Type type);
};

}
