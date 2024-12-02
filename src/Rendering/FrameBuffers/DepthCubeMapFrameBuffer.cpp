#include <Rendering/FrameBuffers/DepthCubeMapFrameBuffer.hpp>

#include <iostream>

namespace shkyera {

DepthCubeMapFrameBuffer::DepthCubeMapFrameBuffer()
    : _width(1024), _height(1024) {
    setupFramebuffer();

    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

DepthCubeMapFrameBuffer::~DepthCubeMapFrameBuffer() {
    if (_fbo != 0) {
        glDeleteFramebuffers(1, &_fbo);
    }
    if (_rbo != 0) {
        glDeleteRenderbuffers(1, &_rbo);
    }
    if (_cubemapTexture != 0) {
        glDeleteTextures(1, &_cubemapTexture);
    }
}

void DepthCubeMapFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void DepthCubeMapFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthCubeMapFrameBuffer::clear() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DepthCubeMapFrameBuffer::activate(GLuint texId) const {
    glActiveTexture(GL_TEXTURE0 + texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);
}

void DepthCubeMapFrameBuffer::setSize(int width, int height) {
    if (width == _width && height == _height) return;

    _width = width;
    _height = height;

    glDeleteTextures(1, &_cubemapTexture);

    glGenTextures(1, &_cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (_rbo != 0) {
        glDeleteRenderbuffers(1, &_rbo);
    }

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Depth Cubemap Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthCubeMapFrameBuffer::setupFramebuffer() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Depth Cubemap Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
