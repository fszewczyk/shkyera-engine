#pragma once

#include <memory>
#include <glad/glad.h>

#include <Rendering/Texture.hpp>

namespace shkyera {

class DepthFrameBuffer {
public:
    DepthFrameBuffer(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
                GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    ~DepthFrameBuffer();

    DepthFrameBuffer(const DepthFrameBuffer& other) = delete;
    DepthFrameBuffer& operator=(const DepthFrameBuffer& other) = delete;

    DepthFrameBuffer(DepthFrameBuffer&& other) noexcept;
    DepthFrameBuffer& operator=(DepthFrameBuffer&& other) noexcept;

    void bind();
    void bind(int index);
    void unbind();
    void clear();

    void setSize(uint32_t width, uint32_t height);
    const Texture& getTexture() const { return _textureDepthBuffer; }

private:
    void setupFramebuffer();

    int _width, _height;
    GLuint _fbo, _rbo;
    Texture _textureDepthBuffer;
};


}
