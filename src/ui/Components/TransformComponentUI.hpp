#pragma once

#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <Components/TransformComponent.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class TransformComponentUI : public ComponentUI {
   public:
    void setPositionGetter(std::function<glm::vec3&(void)> getter);
    void setOrientationGetter(std::function<glm::vec3&(void)> getter);
    void setScaleGetter(std::function<glm::vec3&(void)> getter);

    void draw() override;

   private:
    std::function<glm::vec3&(void)> _positionGetter;
    std::function<glm::vec3&(void)> _orientationGetter;
    std::function<glm::vec3&(void)> _scaleGetter;
};

}  // namespace shkyera
