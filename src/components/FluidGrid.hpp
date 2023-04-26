#pragma once

#include "glm/vec3.hpp"

#include "components/Component.hpp"
#include "core/Grid.hpp"

namespace shkyera {

class FluidGrid : public Component {
  public:
    FluidGrid(glm::vec2 size = {32, 32});

  private:
    Grid<glm::vec3> _velocityField;
};

} // namespace shkyera
