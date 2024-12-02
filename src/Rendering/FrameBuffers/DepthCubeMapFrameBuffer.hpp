#pragma once

#include <memory>
#include <glad/glad.h>

#include <Rendering/Texture.hpp>

namespace shkyera {

class DepthCubeMapFrameBuffer {
public:
    DepthCubeMapFrameBuffer();
    ~DepthCubeMapFrameBuffer();

    void bind();
    void unbind();
    void clear();
    void activate(GLuint texId) const;
    void setSize(int width, int height);

    GLuint getID() const { return _cubemapTexture; }

private:
    GLuint _fbo, _rbo, _cubemapTexture;
    uint32_t _width, _height;

    void setupFramebuffer();
};

} // namespace shkyera
