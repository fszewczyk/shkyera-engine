#include <iostream>

#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>

namespace shkyera {

SceneFrameBuffer::SceneFrameBuffer(GLenum minFilter, GLenum magFilter,
                         GLenum wrapS, GLenum wrapT)
    : _width(0), _height(0),
      _textureColorBuffer(minFilter, magFilter, wrapS, wrapT) {
    setupFramebuffer();
    setSize(256, 256);
}

SceneFrameBuffer::SceneFrameBuffer(SceneFrameBuffer&& other) noexcept
{
    _fbo = other._fbo;
    _rbo = other._rbo;
    _textureColorBuffer = std::move(other._textureColorBuffer);

    other._fbo = 0;
    other._rbo = 0;
    other._textureColorBuffer = Texture{};
}

SceneFrameBuffer& SceneFrameBuffer::operator=(SceneFrameBuffer&& other) noexcept
{
    if(this != &other)
    {
        if(_fbo != 0)
        {
            glDeleteFramebuffers(1, &_fbo);
        }
        if(_rbo != 0)
        {
            glDeleteRenderbuffers(1, &_rbo);
        }

        _fbo = other._fbo;
        _rbo = other._rbo;
        _textureColorBuffer = std::move(other._textureColorBuffer);

        other._fbo = 0;
        other._rbo = 0;
        other._textureColorBuffer = Texture{};
    }

    return *this;
}

SceneFrameBuffer::~SceneFrameBuffer() {
    if(_fbo != 0)
    {
        glDeleteFramebuffers(1, &_fbo);
    }
    if(_rbo != 0)
    {
        glDeleteRenderbuffers(1, &_rbo);
    }
}

void SceneFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
}

void SceneFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneFrameBuffer::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneFrameBuffer::setSize(uint32_t width, uint32_t height) {
    if (width == _width && height == _height) return;

    _width = width;
    _height = height;

    // Resize texture and renderbuffer
    _textureColorBuffer.setData(GL_RGB, _width, _height, GL_RGB, GL_UNSIGNED_BYTE);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
}

void SceneFrameBuffer::setupFramebuffer() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Attach texture as the color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorBuffer.getID(), 0);

    // Attach a renderbuffer for depth and stencil
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Scene Frame Buffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}