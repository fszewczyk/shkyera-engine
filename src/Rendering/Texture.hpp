#pragma once

#include <glad/glad.h>

namespace shkyera {

class Texture {
public:
    // Constructor with configurable filter and wrap modes
    Texture(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
            GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    ~Texture();

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    // Bind and unbind texture
    void bind() const;
    void unbind() const;

    // Set texture data
    void setData(GLenum internalFormat, uint32_t width, uint32_t height, GLenum format, GLenum type, const void* data = nullptr);

    // Activate this texture on a specified texture unit
    void activate(GLenum textureUnit) const;

    // Get OpenGL texture ID
    GLuint getID() const { return _textureID; }

private:
    GLuint _textureID;
};

}
