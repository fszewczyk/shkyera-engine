#include <Rendering/Texture.hpp>
#include <iostream>

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

Texture::~Texture() {
    if (_textureID) {
        glDeleteTextures(1, &_textureID);
        _textureID = 0;
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
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
