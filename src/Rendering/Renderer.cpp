#include <iostream>

#include <Rendering/Renderer.hpp>
#include <InputManager/InputManager.hpp>
#include <Components/TriangleComponent.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

constexpr static float MOVEMENT_SPEED = 0.1;
constexpr static float MOUSE_SENSITIVITY = 0.1;


Renderer::Renderer(std::shared_ptr<Registry> registry) : _registry(registry) {
    setupCameraMovement();
    setupFramebuffer();
}

Renderer::~Renderer() {
    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(1, &_textureColorBuffer);
    glDeleteRenderbuffers(1, &_rbo);
}

void Renderer::draw() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, (GLsizei)_renderWidth, (GLsizei)_renderHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto& comp : _registry->getComponents<TriangleComponent>()) {
        comp.update();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderer::getTexture() const {
    return _textureColorBuffer;
}

void Renderer::setupCameraMovement() {
    auto& inputManager = InputManager::getInstance();
    auto& camera = _camera;

    inputManager.registerKeyCallback(GLFW_KEY_W, [&camera]() {
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);  // Forward direction in local space
        glm::vec3 worldForward = glm::vec3(glm::rotate(glm::mat4(1.0f), camera.getOrientation().y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(forward, 0.0f));
        camera.setPosition(camera.getPosition() + worldForward * MOVEMENT_SPEED);
    });

    inputManager.registerKeyCallback(GLFW_KEY_S, [&camera]() {
        glm::vec3 backward = glm::vec3(0.0f, 0.0f, 1.0f);  // Backward direction in local space
        glm::vec3 worldBackward = glm::vec3(glm::rotate(glm::mat4(1.0f), camera.getOrientation().y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(backward, 0.0f));
        camera.setPosition(camera.getPosition() + worldBackward * MOVEMENT_SPEED);
    });

    inputManager.registerKeyCallback(GLFW_KEY_A, [&camera]() {
        glm::vec3 left = glm::vec3(-1.0f, 0.0f, 0.0f);  // Left direction in local space
        glm::vec3 worldLeft = glm::vec3(glm::rotate(glm::mat4(1.0f), camera.getOrientation().y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(left, 0.0f));
        camera.setPosition(camera.getPosition() + worldLeft * MOVEMENT_SPEED);
    });

    inputManager.registerKeyCallback(GLFW_KEY_D, [&camera]() {
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);  // Right direction in local space
        glm::vec3 worldRight = glm::vec3(glm::rotate(glm::mat4(1.0f), camera.getOrientation().y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(right, 0.0f));
        camera.setPosition(camera.getPosition() + worldRight * MOVEMENT_SPEED);
    });

    inputManager.registerKeyCallback(GLFW_KEY_Q, [&camera]() {
        glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);  // Down direction in world space
        camera.setPosition(camera.getPosition() + down * MOVEMENT_SPEED);
    });

    inputManager.registerKeyCallback(GLFW_KEY_E, [&camera]() {
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up direction in world space
        camera.setPosition(camera.getPosition() + up * MOVEMENT_SPEED);
    });

    inputManager.registerMouseMoveCallback([&camera](double xPos, double yPos) {
        static double lastX = xPos, lastY = yPos;

        float xOffset = static_cast<float>(xPos - lastX) * MOUSE_SENSITIVITY;
        float yOffset = static_cast<float>(lastY - yPos) * MOUSE_SENSITIVITY;  // Y is inverted in OpenGL

        lastX = xPos;
        lastY = yPos;

        glm::vec3 orientation = camera.getOrientation();
        orientation.y += glm::radians(xOffset);  // Yaw (Y-axis rotation)
        orientation.x += glm::radians(yOffset);  // Pitch (X-axis rotation)

        if (orientation.x > glm::radians(89.0f))
            orientation.x = glm::radians(89.0f);
        if (orientation.x < glm::radians(-89.0f))
            orientation.x = glm::radians(-89.0f);

        camera.getOrientation() = orientation;
    });
}

void Renderer::setupFramebuffer() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create a texture to render the scene into
    glGenTextures(1, &_textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, _textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)_renderWidth, (GLsizei)_renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorBuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)_renderWidth, (GLsizei)_renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::setSize(uint32_t width, uint32_t height) {
    _renderWidth = width;
    _renderHeight = height;

    glBindTexture(GL_TEXTURE_2D, _textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
}

}
