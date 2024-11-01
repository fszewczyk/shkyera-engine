#include <Rendering/Texture.hpp>
#include <iostream>

namespace shkyera {

Texture::Texture(GLenum target, GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT)
    : _target(target), _minFilter(minFilter), _magFilter(magFilter), _wrapS(wrapS), _wrapT(wrapT) {
    glGenTextures(1, &_textureID);
    initializeParameters();
}

Texture::~Texture() {
    cleanup();
}

void Texture::bind() const {
    glBindTexture(_target, _textureID);
}

void Texture::unbind() const {
    glBindTexture(_target, 0);
}

void Texture::setData(GLenum internalFormat, uint32_t width, uint32_t height, GLenum format, GLenum type, const void* data) {
    bind();
    glTexImage2D(_target, 0, internalFormat, width, height, 0, format, type, data);
    unbind();
}

void Texture::activate(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    bind();
}

void Texture::cleanup() {
    if (_textureID) {
        glDeleteTextures(1, &_textureID);
        _textureID = 0;
    }
}

void Texture::initializeParameters() {
    bind();
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _minFilter);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, _wrapS);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, _wrapT);
    unbind();
}

}
