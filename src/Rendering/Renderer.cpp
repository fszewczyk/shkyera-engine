#include <iostream>

#include <Rendering/Renderer.hpp>
#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Mesh.hpp>
#include <InputManager/InputManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/MeshComponent.hpp>
#include <Components/CameraComponent.hpp>

namespace shkyera {

constexpr static float MOVEMENT_SPEED = 0.6;
constexpr static float MOUSE_SENSITIVITY = 0.0;


Renderer::Renderer(std::shared_ptr<Registry> registry) : _registry(registry), _cameraControl(false) {
    _camera = registry->addEntity();
    registry->addComponent<TransformComponent>(_camera);
    registry->addComponent<CameraComponent>(_camera);

    setupFramebuffer();
    setupCameraMovement();

    InputManager::getInstance().registerMouseButtonDownCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        enableCameraMovement();
    });
    InputManager::getInstance().registerMouseButtonUpCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        disableCameraMovement();
    });

    auto vertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/camera_view.glsl", Shader::Type::Vertex);
    auto fragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/orange.glsl", Shader::Type::Fragment);

    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    _shaderProgram.link();
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

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_camera).getViewMatrix();
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_camera).getProjectionMatrix();

    _shaderProgram.use();
    _shaderProgram.setUniform("viewMatrix", viewMatrix);
    _shaderProgram.setUniform("projectionMatrix", projectionMatrix);

    for(const auto& [entity, meshComponent] : _registry->getComponentSet<MeshComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _shaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());
        meshComponent.updateImpl();
    }

    _shaderProgram.stopUsing();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderer::getTexture() const {
    return _textureColorBuffer;
}

void Renderer::enableCameraMovement() {
    _cameraControl = true;
}
void Renderer::disableCameraMovement() {
    _cameraControl = false;
}

void Renderer::setupCameraMovement() {
    auto& inputManager = InputManager::getInstance();
    auto& cameraTransform = _registry->getComponent<CameraComponent>(_camera);

    inputManager.registerKeyCallback(GLFW_KEY_W, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 forward;
            float yaw = cameraTransform.getOrientation().y;
            float pitch = cameraTransform.getOrientation().x;

            forward.x = cos(yaw) * cos(pitch);
            forward.y = sin(pitch);
            forward.z = sin(yaw) * cos(pitch);
            forward = glm::normalize(forward);

            cameraTransform.setPosition(cameraTransform.getPosition() + forward * MOVEMENT_SPEED);
        }
    });

    inputManager.registerKeyCallback(GLFW_KEY_S, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 backward;
            float yaw = cameraTransform.getOrientation().y;
            float pitch = cameraTransform.getOrientation().x;

            backward.x = -cos(yaw) * cos(pitch);
            backward.y = -sin(pitch);
            backward.z = -sin(yaw) * cos(pitch);
            backward = glm::normalize(backward);

            cameraTransform.setPosition(cameraTransform.getPosition() + backward * MOVEMENT_SPEED);
        }
    });

    inputManager.registerKeyCallback(GLFW_KEY_A, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 left;
            float yaw = cameraTransform.getOrientation().y;

            left.x = -sin(yaw);
            left.y = 0.0f;
            left.z = cos(yaw);
            left = glm::normalize(left);

            cameraTransform.setPosition(cameraTransform.getPosition() + left * MOVEMENT_SPEED);
        }
    });

    inputManager.registerKeyCallback(GLFW_KEY_D, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 right;
            float yaw = cameraTransform.getOrientation().y;

            right.x = sin(yaw);
            right.y = 0.0f;
            right.z = -cos(yaw);
            right = glm::normalize(right);

            cameraTransform.setPosition(cameraTransform.getPosition() + right * MOVEMENT_SPEED);
        }
    });


    inputManager.registerKeyCallback(GLFW_KEY_Q, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);  // Down direction in world space
            cameraTransform.setPosition(cameraTransform.getPosition() + down * MOVEMENT_SPEED);
        }
    });

    inputManager.registerKeyCallback(GLFW_KEY_E, [&cameraTransform, &enabled=_cameraControl]() {
        if(enabled) {
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up direction in world space
            cameraTransform.setPosition(cameraTransform.getPosition() + up * MOVEMENT_SPEED);
        }
    });

    inputManager.registerMouseMoveCallback([&cameraTransform, &enabled=_cameraControl](double xPos, double yPos) {
        static double lastX = xPos, lastY = yPos;

        float xOffset = static_cast<float>(xPos - lastX) * MOUSE_SENSITIVITY;
        float yOffset = static_cast<float>(lastY - yPos) * MOUSE_SENSITIVITY;  // Y is inverted in OpenGL

        lastX = xPos;
        lastY = yPos;

        if(enabled) {
            glm::vec3 orientation = cameraTransform.getOrientation();
            orientation.y -= glm::radians(xOffset);  // Yaw (Y-axis rotation)
            orientation.x += glm::radians(yOffset);  // Pitch (X-axis rotation)

            if (orientation.x > glm::radians(89.0f))
                orientation.x = glm::radians(89.0f);
            if (orientation.x < glm::radians(-89.0f))
                orientation.x = glm::radians(-89.0f);

            cameraTransform.getOrientation() = orientation;
        }
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
