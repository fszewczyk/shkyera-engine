#pragma once

#include <glad/glad.h>
#include <Rendering/ShaderProgram.hpp>
#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>

namespace shkyera::utils
{

void drawFullscreenQuad();

template<typename ValueType>
std::pair<std::string, ValueType> Uniform(const std::string& name, const ValueType& value)
{
    return std::make_pair(name, value);
}

template <typename... Uniforms>
void applyShaderToFrameBuffer(
    SceneFrameBuffer& frameBuffer,
    ShaderProgram& shaderProgram,
    std::vector<std::pair<const char *, const Texture *>> textures, // List of texture bindings with texture unit indices
    Uniforms... uniforms) // Parameter pack for uniform values
{
    // Bind framebuffer and activate shader program
    frameBuffer.bind();
    UseShader shaderUsage(shaderProgram);

    // Bind textures to their corresponding texture units
    int textureIndex = 0;
    for (const auto& [name, texture] : textures) {
        texture->activate(GL_TEXTURE0 + textureIndex);
        shaderProgram.setUniform(name, textureIndex++);
    }

    // Set uniforms using the parameter pack
    (shaderProgram.setUniform(uniforms.first, uniforms.second), ...);

    // Draw fullscreen quad
    utils::drawFullscreenQuad();

    // Stop using the shader program and unbind framebuffer
    frameBuffer.unbind();
}

template <typename ArrayType, typename... Uniforms>
void applyShaderToFrameBuffer(
    SceneFrameBuffer& frameBuffer,
    ShaderProgram& shaderProgram,
    std::vector<std::pair<const char *, const Texture *>> textures, // List of texture bindings with texture unit indices
    std::pair<const char *, std::vector<ArrayType>> array,
    Uniforms... uniforms) // Parameter pack for uniform values
{
    // Bind framebuffer and activate shader program
    frameBuffer.bind();
    shaderProgram.use();

    // Bind textures to their corresponding texture units
    int textureIndex = 0;
    for (const auto& [name, texture] : textures) {
        texture->activate(GL_TEXTURE0 + textureIndex);
        shaderProgram.setUniform(name, textureIndex++);
    }

    int index = 0;
    for(const auto& val : array.second)
    {
        shaderProgram.setUniform(std::string(array.first) + "[" + std::to_string(index++) + "]", val);
    }

    // Set uniforms using the parameter pack
    (shaderProgram.setUniform(uniforms.first, uniforms.second), ...);

    // Draw fullscreen quad
    utils::drawFullscreenQuad();

    // Stop using the shader program and unbind framebuffer
    shaderProgram.stopUsing();
    frameBuffer.unbind();
}

}
