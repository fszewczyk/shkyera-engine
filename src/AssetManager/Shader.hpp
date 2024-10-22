#pragma once

#include <glad/glad.h>

#include <AssetManager/Asset.hpp>

namespace shkyera {

class Shader : public Asset {
public:
    enum class Type {
        Vertex,
        Fragment
    };

    Shader(const std::string& filepath, Type type);
    
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
