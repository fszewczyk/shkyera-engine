#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <AssetManager/Texture.hpp>

namespace shkyera {

class SceneFrameBuffer {
public:
    SceneFrameBuffer(GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR,
                GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    ~SceneFrameBuffer();

    SceneFrameBuffer(const SceneFrameBuffer& other) = delete;
    SceneFrameBuffer& operator=(const SceneFrameBuffer& other) = delete;

    SceneFrameBuffer(SceneFrameBuffer&& other) noexcept;
    SceneFrameBuffer& operator=(SceneFrameBuffer&& other) noexcept;

    void bind();
    void unbind();
    void clear();

    void setSize(uint32_t width, uint32_t height);
    glm::vec2 getSize() const;
    const Texture& getTexture() const { return _textureColorBuffer; }

private:
    void setupFramebuffer();

    int _width, _height;
    GLuint _fbo, _rbo;
    Texture _textureColorBuffer;
};


}
