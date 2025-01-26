#pragma once

#include <glad/glad.h>
#include <AssetManager/Image.hpp>

namespace shkyera {

class Texture : public PathConstructibleAsset<Texture> {
public:
    Texture(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    Texture(const std::filesystem::path& path);
    Texture(std::shared_ptr<Image> image);

    ~Texture();

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Bind and unbind texture
    void bind() const;
    void unbind() const;

    bool loadImage(std::shared_ptr<Image> imageAsset);

    // Set texture data
    void setData(GLenum internalFormat, uint32_t width, uint32_t height, GLenum format, GLenum type, const void* data = nullptr);

    // Activate this texture on a specified texture unit
    void activate(GLenum textureUnit) const;

    GLuint getID() const { return _textureID; }

    void* getImguiTextureID() const { return reinterpret_cast<void *>(_textureID); }

private:
    void generateTexture(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    GLuint _textureID;
};

}
