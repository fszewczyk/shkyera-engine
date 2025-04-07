#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include <AssetManager/Shader.hpp>

namespace shkyera {

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();

  void attachShader(const std::shared_ptr<Shader>& shader);

  void link();

  void use();
  void stopUsing();

  void setUniform(const std::string& name, int value);
  void setUniform(const std::string& name, float value);
  void setUniform(const std::string& name, const glm::vec2& value);
  void setUniform(const std::string& name, const glm::vec3& value);
  void setUniform(const std::string& name, const glm::mat4& value);

 private:
  bool isInUse();
  GLint getUniformLocation(const std::string& name);

  GLuint _id;  // OpenGL shader program ID
  std::unordered_map<std::string, GLint> _uniformLocationCache;

  inline static std::stack<GLuint> _shadersInUse{};
};

class UseShader {
 public:
  UseShader(ShaderProgram& shaderProgram);
  ~UseShader();

 private:
  ShaderProgram& mShaderProgram;
};

}  // namespace shkyera