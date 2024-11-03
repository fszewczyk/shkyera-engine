#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

#include <AssetManager/Image.hpp>

namespace shkyera {

Image::Image(const std::string& path) {
  load(path);
}

void Image::load(const std::string& path) {
  _data = stbi_load(path.c_str(), &_width, &_height, &_components,
                    STBI_rgb_alpha);

  if (_data == nullptr)
    throw std::invalid_argument("Could not load image at: " + path);
}

void Image::save(const std::string& path) const {
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

std::string Image::ICON_CONSOLE_TOTAL = "resources/icons/console/total.png";
std::string Image::ICON_CONSOLE_ERROR = "resources/icons/console/error.png";
std::string Image::ICON_CONSOLE_INFO = "resources/icons/console/info.png";
std::string Image::ICON_CONSOLE_VERBOSE = "resources/icons/console/verbose.png";
std::string Image::ICON_CONSOLE_SUCCESS = "resources/icons/console/success.png";
std::string Image::ICON_COMPONENT_TRANSFORM = "resources/icons/components/transform.png";
std::string Image::ICON_COMPONENT_SCRIPT = "resources/icons/components/script.png";
std::string Image::ICON_COMPONENT_SHAPE = "resources/icons/components/shape.png";
std::string Image::ICON_FILES_FOLDER = "resources/icons/files/folder.png";
std::string Image::ICON_FILES_PYTHON = "resources/icons/files/python.png";
std::string Image::ICON_FILES_IMAGE = "resources/icons/files/image.png";
std::string Image::ICON_FILES_TEXT = "resources/icons/files/text.png";
std::string Image::ICON_BUTTON_PLAY = "resources/icons/buttons/play.png";
std::string Image::ICON_BUTTON_STOP = "resources/icons/buttons/stop.png";


}  // namespace shkyera
