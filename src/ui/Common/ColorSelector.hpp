#pragma once

#include <functional>
#include <string>

#include <glm/glm.hpp>
#include "imgui.h"

namespace shkyera {

class ColorSelector {
 public:
  ColorSelector(const std::string& title);
  ColorSelector(const std::string& title, const glm::vec3& color);

  void setUpdateCallback(std::function<void(const glm::vec3& color)> callback);

  void draw();

 private:
  std::string _title;
  std::string _imguiIdentifier;
  glm::vec3 _color;
  std::function<void(const glm::vec3& file)> _updateCallback;
};

}  // namespace shkyera
