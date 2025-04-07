#pragma once

#include <glad/glad.h>
#include <filesystem>

namespace shkyera {

class Shader {
 public:
  enum class Type {
    Vertex,
    Geometry,
    Fragment,
  };

  Shader(const std::filesystem::path& filepath, Type type);
  Shader(const Shader& other) = delete;
  Shader& operator=(const Shader& other) = delete;
  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  ~Shader();

  GLuint getID() const { return _id; }

 private:
  GLuint _id;
  Type _type;

  std::string loadFromFile(const std::string& filepath);

  void compile(const std::string& source);

  GLenum shaderTypeToGLenum(Type type);
};

}  // namespace shkyera
