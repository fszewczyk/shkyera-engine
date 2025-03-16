#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include <AssetManager/Image.hpp>

namespace shkyera {

Image::Image(const std::filesystem::path& path)
    : PathConstructibleAsset(path) {
    load(path);
}

Image::Image(Image&& other) noexcept
    : PathConstructibleAsset(std::move(other)),
      _data(std::exchange(other._data, nullptr)),
      _width(std::exchange(other._width, 0)),
      _height(std::exchange(other._height, 0)),
      _components(std::exchange(other._components, 0)) {}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        PathConstructibleAsset::operator=(std::move(other));

        delete[] _data;

        _data = std::exchange(other._data, nullptr);
        _width = std::exchange(other._width, 0);
        _height = std::exchange(other._height, 0);
        _components = std::exchange(other._components, 0);
    }
    return *this;
}

void Image::load(const std::filesystem::path& path) {
    _data = stbi_load(path.c_str(), &_width, &_height, &_components, 0);

    if (_data == nullptr) {
        throw std::invalid_argument(std::string("Could not load image at: ") + path.c_str());
    }
}

void Image::save(const std::filesystem::path& path) const {
    stbi_write_png(path.c_str(), _width, _height, _components, _data,
                   _components * _width);
}

uint8_t const* Image::getData() const {
    return _data;
}

int Image::getWidth() const {
    return _width;
}
int Image::getHeight() const {
    return _height;
}
int Image::getChannels() const {
    return _components;
}

std::filesystem::path Image::ICON_CONSOLE_TOTAL = "resources/icons/console/total.png";
std::filesystem::path Image::ICON_CONSOLE_ERROR = "resources/icons/console/error.png";
std::filesystem::path Image::ICON_CONSOLE_INFO = "resources/icons/console/info.png";
std::filesystem::path Image::ICON_CONSOLE_VERBOSE = "resources/icons/console/verbose.png";
std::filesystem::path Image::ICON_CONSOLE_SUCCESS = "resources/icons/console/success.png";
std::filesystem::path Image::ICON_COMPONENT_TRANSFORM = "resources/icons/components/transform.png";
std::filesystem::path Image::ICON_COMPONENT_SCRIPT = "resources/icons/components/script.png";
std::filesystem::path Image::ICON_COMPONENT_SHAPE = "resources/icons/components/shape.png";
std::filesystem::path Image::ICON_FILES_FOLDER = "resources/icons/files/folder.png";
std::filesystem::path Image::ICON_FILES_PYTHON = "resources/icons/files/python.png";
std::filesystem::path Image::ICON_FILES_IMAGE = "resources/icons/files/image.png";
std::filesystem::path Image::ICON_FILES_TEXT = "resources/icons/files/text.png";
std::filesystem::path Image::ICON_BUTTON_PLAY = "resources/icons/buttons/play.png";
std::filesystem::path Image::ICON_BUTTON_STOP = "resources/icons/buttons/stop.png";

}  // namespace shkyera
