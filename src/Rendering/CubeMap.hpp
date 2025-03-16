#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

#include <AssetManager/Image.hpp>

namespace shkyera {

class CubeMap {
   public:
    // Constructor with configurable filter and wrap modes for cube maps
    CubeMap(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
            GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE, GLenum wrapR = GL_CLAMP_TO_EDGE);
    ~CubeMap();

    CubeMap(const CubeMap& other) = delete;
    CubeMap& operator=(const CubeMap& other) = delete;

    CubeMap(CubeMap&& other) noexcept;
    CubeMap& operator=(CubeMap&& other) noexcept;

    void initialize(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
                    GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE, GLenum wrapR = GL_CLAMP_TO_EDGE);

    // Bind and unbind the cube map
    void bind() const;
    void unbind() const;

    // Load data for each face of the cube map from file paths
    bool loadFaces(std::shared_ptr<Image> up, std::shared_ptr<Image> down, std::shared_ptr<Image> left, std::shared_ptr<Image> right, std::shared_ptr<Image> front, std::shared_ptr<Image> back, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

    // Set texture data for each cube face from memory
    void setFaceData(GLenum face, GLenum internalFormat, uint32_t width, uint32_t height, GLenum format, GLenum type, const void* data);

    // Activate this cube map on a specified texture unit
    void activate(GLenum textureUnit) const;

    // Get OpenGL texture ID
    GLuint getID() const { return _textureID; }

   private:
    GLuint _textureID;
};

}  // namespace shkyera
