#pragma once

#include <filesystem>

#include "glm/vec3.hpp"

#include "core/Grid.hpp"

namespace shkyera {

class Image {
  public:
    Image(Grid<glm::vec3> grid);
    Image(size_t width, size_t height);
    Image(std::filesystem::path path);

    void load(std::filesystem::path path);

    void saveToPng(std::filesystem::path path) const;
    void saveToJpg(std::filesystem::path path) const;

    void updateTextureId();
    uint64_t getTextureId() const;

    inline glm::vec3 at(size_t x, size_t y) const;

    static Image ICON_LOGO;
    static Image ICON_INFO;
    static Image ICON_ERROR;
    static Image ICON_SUCCESS;
    static Image ICON_VERBOSE;

  private:
    size_t _width;
    size_t _height;

    std::vector<std::vector<glm::vec3>> _data;

    bool _assignedTextureId;
    uint64_t _textureId;
};

} // namespace shkyera
