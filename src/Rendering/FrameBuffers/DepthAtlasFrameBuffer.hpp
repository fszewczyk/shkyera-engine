#pragma once

#include <memory>
#include <glad/glad.h>

#include <AssetManager/Texture.hpp>

namespace shkyera {

class DepthAtlasFrameBuffer {
public:
    DepthAtlasFrameBuffer(int texturesInAtlas, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
                GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    ~DepthAtlasFrameBuffer();

    DepthAtlasFrameBuffer(const DepthAtlasFrameBuffer& other) = delete;
    DepthAtlasFrameBuffer& operator=(const DepthAtlasFrameBuffer& other) = delete;

    DepthAtlasFrameBuffer(DepthAtlasFrameBuffer&& other) noexcept;
    DepthAtlasFrameBuffer& operator=(DepthAtlasFrameBuffer&& other) noexcept;

    void bind();
    void bind(int index);
    void unbind();
    void clear();

    void setSize(uint32_t width, uint32_t height);
    const Texture& getTexture() const { return _textureDepthBuffer; }

    int getNumberOfTextures() const;

private:
    void setupFramebuffer();

    int _texturesInAtlas;
    int _width, _height;
    GLuint _fbo, _rbo;
    Texture _textureDepthBuffer;
};


}
