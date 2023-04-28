#pragma once

#include <filesystem>

namespace shkyera {

class Image {
  public:
    Image(size_t width, size_t height);
    Image(std::filesystem::path path);

    void load(std::filesystem::path path);
    void save(std::filesystem::path path) const;

    void updateTextureId();
    uint64_t getTextureId() const;

    static Image ICON_CONSOLE_TOTAL;
    static Image ICON_CONSOLE_ERROR;
    static Image ICON_CONSOLE_INFO;
    static Image ICON_CONSOLE_VERBOSE;
    static Image ICON_CONSOLE_SUCCESS;

  private:
    uint8_t *_data;

    int _width;
    int _height;
    int _components;

    bool _wasAssignedTextureId;
    uint64_t _textureId;
};

} // namespace shkyera
