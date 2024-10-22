#include <iostream>

#include <Systems/RenderingSystem.hpp>
#include <Components/CameraComponent.hpp> 

namespace shkyera {

RenderingSystem::RenderingSystem(std::shared_ptr<Registry> registry)
    : _registry(registry), _width(0), _height(0) {
    setupFramebuffer();
}

void RenderingSystem::startFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void RenderingSystem::endFrame() {
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::setSize(uint32_t width, uint32_t height) {
    if(width == _width && height == _height) {
        return;
    }

    _width = width;
    _height = height;

    float aspectRatio = static_cast<float>(width) / height;
    _registry->getComponent<CameraComponent>(_registry->getCamera()).aspectRatio = aspectRatio;

    glBindTexture(GL_TEXTURE_2D, _textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)_width, (GLsizei)_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)_width, (GLsizei)_height);
}

GLuint RenderingSystem::getTexture() const {
    return _textureColorBuffer;
}

void RenderingSystem::setupFramebuffer() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, _textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)_width, (GLsizei)_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorBuffer, 0);

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)_width, (GLsizei)_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::cleanupFramebuffer() {
    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(1, &_textureColorBuffer);
    glDeleteRenderbuffers(1, &_rbo);
}

}
