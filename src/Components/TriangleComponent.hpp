#pragma once

#include <iostream>

#include <Components/BaseComponent.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <AssetManager/Shader.hpp>
#include <AssetManager/AssetManager.hpp>

namespace shkyera {

class TriangleComponent : public BaseComponent<TriangleComponent> {
public:
    TriangleComponent() {
        setupShaders();
        setupTriangle();
    }

    TriangleComponent(const TriangleComponent& other) = delete;
    TriangleComponent& operator=(const TriangleComponent& other) = delete;

    TriangleComponent(TriangleComponent&& other) noexcept 
        : _vao(other._vao), _vbo(other._vbo), _shaderProgram(std::move(other._shaderProgram)) {
        other._vao = 0;
        other._vbo = 0;
    }

    TriangleComponent& operator=(TriangleComponent&& other) noexcept {
        if (this != &other) {
            cleanup();

            _vao = other._vao;
            _vbo = other._vbo;
            _shaderProgram = std::move(other._shaderProgram);

            other._vao = 0;
            other._vbo = 0;
        }
        return *this;
    }

    ~TriangleComponent() {
        cleanup();
    }

    void updateImpl() {
        render();
    }

private:
    GLuint _vao, _vbo;
    std::shared_ptr<ShaderProgram> _shaderProgram;

    void setupTriangle() {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,  // left
             0.5f, -0.5f, 0.0f,  // right
             0.0f,  0.5f, 0.0f   // top
        };

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);

        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void setupShaders() {
        // Load and compile shaders using AssetManager
        auto vertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex_shader.glsl", Shader::Type::Vertex);
        auto fragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment_shader.glsl", Shader::Type::Fragment);

        _shaderProgram = std::make_shared<ShaderProgram>();
        _shaderProgram->attachShader(vertexShader);
        _shaderProgram->attachShader(fragmentShader);
        _shaderProgram->link();
    }

    void render() {
        _shaderProgram->use();
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void cleanup() {
        if (_vao != 0) {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
        if (_vbo != 0) {
            glDeleteBuffers(1, &_vbo);
            _vbo = 0;
        }
    }
};

} // namespace shkyera
