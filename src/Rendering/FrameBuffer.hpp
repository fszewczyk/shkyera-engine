#pragma once

#include <memory>
#include <glad/glad.h>

#include <Rendering/Texture.hpp>

namespace shkyera {

class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();

    void bind();
    void unbind();
    void clear();

    void setSize(uint32_t width, uint32_t height);
    const Texture& getTexture() const { return _textureColorBuffer; }

private:
    void setupFramebuffer();

    GLuint _fbo, _rbo;
    uint32_t _width, _height;
    Texture _textureColorBuffer;
};


}
