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

Image::Image(const std::string& path) : _wasAssignedTextureId(false) {
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

void Image::updateTextureId() {
  uint32_t textureId = _textureId;

  if (_wasAssignedTextureId)
    glDeleteTextures(1, &textureId);

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (_components == 3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, _data);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, _data);

  _wasAssignedTextureId = true;
  _textureId = textureId;
}

uint64_t Image::getTextureId() const {
  return _textureId;
}

Image Image::ICON_CONSOLE_TOTAL = Image("resources/icons/console/total.png");
Image Image::ICON_CONSOLE_ERROR = Image("resources/icons/console/error.png");
Image Image::ICON_CONSOLE_INFO = Image("resources/icons/console/info.png");
Image Image::ICON_CONSOLE_VERBOSE =
    Image("resources/icons/console/verbose.png");
Image Image::ICON_CONSOLE_SUCCESS =
    Image("resources/icons/console/success.png");

Image Image::ICON_COMPONENT_TRANSFORM =
    Image("resources/icons/components/transform.png");
Image Image::ICON_COMPONENT_SCRIPT =
    Image("resources/icons/components/script.png");
Image Image::ICON_COMPONENT_SHAPE =
    Image("resources/icons/components/shape.png");

Image Image::ICON_FILES_FOLDER = Image("resources/icons/files/folder.png");
Image Image::ICON_FILES_PYTHON = Image("resources/icons/files/python.png");
Image Image::ICON_FILES_IMAGE = Image("resources/icons/files/image.png");
Image Image::ICON_FILES_TEXT = Image("resources/icons/files/text.png");

Image Image::ICON_BUTTON_PLAY = Image("resources/icons/buttons/play.png");
Image Image::ICON_BUTTON_STOP = Image("resources/icons/buttons/stop.png");

}  // namespace shkyera
