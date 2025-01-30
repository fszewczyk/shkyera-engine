#include <iostream>

#include <Common/Logger.hpp>
#include <Rendering/FrameBuffers/DepthAtlasFrameBuffer.hpp>

namespace shkyera {

DepthAtlasFrameBuffer::DepthAtlasFrameBuffer(int texturesInAtlas, GLenum minFilter, GLenum magFilter,
                         GLenum wrapS, GLenum wrapT)
    : _texturesInAtlas(texturesInAtlas), _width(2048), _height(2048),
      _textureDepthBuffer(minFilter, magFilter, wrapS, wrapT) {
    setupFramebuffer();
}

DepthAtlasFrameBuffer::DepthAtlasFrameBuffer(DepthAtlasFrameBuffer&& other) noexcept
{
    _texturesInAtlas = other._texturesInAtlas;
    _width = other._width;
    _height = other._height;
    _fbo = other._fbo;
    _rbo = other._rbo;
    _textureDepthBuffer = std::move(other._textureDepthBuffer);

    other._fbo = 0;
    other._rbo = 0;
}

DepthAtlasFrameBuffer& DepthAtlasFrameBuffer::operator=(DepthAtlasFrameBuffer&& other) noexcept
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

        _texturesInAtlas = other._texturesInAtlas;
        _width = other._width;
        _height = other._height;
        _fbo = other._fbo;
        _rbo = other._rbo;
        _textureDepthBuffer = std::move(other._textureDepthBuffer);

        other._fbo = 0;
        other._rbo = 0;
    }

    return *this;
}

DepthAtlasFrameBuffer::~DepthAtlasFrameBuffer() {
    if(_fbo != 0)
    {
        glDeleteFramebuffers(1, &_fbo);
    }
    if(_rbo != 0)
    {
        glDeleteRenderbuffers(1, &_rbo);
    }
}

int DepthAtlasFrameBuffer::getNumberOfTextures() const {
    return _texturesInAtlas;
}


void DepthAtlasFrameBuffer::bind(int index) {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(static_cast<GLsizei>(index * _width / _texturesInAtlas), 0, static_cast<GLsizei>(_width / _texturesInAtlas), static_cast<GLsizei>(_height));
}

void DepthAtlasFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
}

void DepthAtlasFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthAtlasFrameBuffer::clear() {
    bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    unbind();
}

void DepthAtlasFrameBuffer::setSize(uint32_t width, uint32_t height) {
    if (width == _width && height == _height) return;

    _width = width;
    _height = height;

    _textureDepthBuffer.setData(GL_R32F, _width, _height, GL_RED, GL_FLOAT);

    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
}

void DepthAtlasFrameBuffer::setupFramebuffer() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureDepthBuffer.getID(), 0);

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    glBindTexture(GL_TEXTURE_2D, _textureDepthBuffer.getID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::INFO("Depth Frame Buffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
