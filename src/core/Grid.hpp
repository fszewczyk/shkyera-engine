#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

namespace shkyera {

template <typename T> class Grid {
  public:
    Grid();
    Grid(glm::vec<2, size_t> size);
    Grid(glm::vec<3, size_t> size);
    Grid(glm::vec<3, size_t> size, float cellSize);

    T &operator()(size_t x, size_t y, size_t z);
    T at(size_t x, size_t y, size_t z);

  private:
    float _cellSize;
    std::vector<std::vector<std::vector<T>>> _grid;
};

} // namespace shkyera

#include "core/Grid.inl"
