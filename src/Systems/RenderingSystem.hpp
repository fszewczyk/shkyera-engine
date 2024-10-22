#pragma once

#include <memory>
#include <glad/glad.h>
#include <ECS/Registry.hpp>


namespace shkyera {

class RenderingSystem {
public:
    RenderingSystem(std::shared_ptr<Registry> registry);

    void startFrame();
    void endFrame();
    void setSize(uint32_t width, uint32_t height);
    GLuint getTexture() const;

private:
    void setupFramebuffer();
    void cleanupFramebuffer();

    std::shared_ptr<Registry> _registry;
    GLuint _fbo, _textureColorBuffer, _rbo;
    uint32_t _width, _height;
};

}
