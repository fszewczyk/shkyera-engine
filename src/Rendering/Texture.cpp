#include <iostream>

#include <Common/Logger.hpp>
#include <Rendering/Texture.hpp>
#include <AssetManager/AssetManager.hpp>


namespace shkyera {

Texture::Texture(GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT)
{
    glGenTextures(1, &_textureID);
    
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    unbind();
}

Texture::Texture(const std::string& path) : Texture()
{
    const auto& image = AssetManager::getInstance().getAsset<Image>(path);
    loadImage(image);
}

Texture::Texture(Texture&& other) noexcept
{
    _textureID = other._textureID;
    other._textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if(this != &other)
    {
        if(_textureID)
        {
            glDeleteTextures(1, &_textureID);
        }
        _textureID = other._textureID;
        other._textureID = 0;
    }

    return *this;
}

Texture::~Texture() {

    if (_textureID) 
    {
        glDeleteTextures(1, &_textureID);
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::loadImage(std::shared_ptr<Image> imageAsset)
{
    if (auto* data = imageAsset->getData()) 
    {
        auto width = imageAsset->getWidth();
        auto height = imageAsset->getHeight();
        auto channels = imageAsset->getChannels();

        bind();
        if (channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                        GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, data);        
        }
        unbind();
    }
    else
    {
        Logger::ERROR("Image was not loaded. Could not load cubemap.");
        return false;
    }

    return true;
}

void Texture::setData(GLenum internalFormat, uint32_t width, uint32_t height, GLenum format, GLenum type, const void* data) {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    unbind();
}

void Texture::activate(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    bind();
}

}
