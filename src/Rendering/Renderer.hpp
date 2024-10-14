#pragma once

#include <glad/glad.h>

#include <ECS/Registry.hpp>
#include <Rendering/Renderer.hpp>

namespace shkyera {

class Renderer {
    public:
        Renderer(std::shared_ptr<Registry> registry);
        ~Renderer();

        void draw();

        GLuint getTexture() const;

        void setSize(uint32_t width, uint32_t height);

    private:
        void drawScene();
        void setupFramebuffer();
        void cleanupFramebuffer();

        std::shared_ptr<Registry> _registry;
        uint32_t _renderWidth;
        uint32_t _renderHeight;

        GLuint _fbo = 0;
        GLuint _textureColorBuffer = 0;
        GLuint _rbo = 0;
};

}
