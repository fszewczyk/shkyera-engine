#pragma once

#include <filesystem>

namespace shkyera {

/**
 * @class Image
 * @brief A class for working with images and textures.
 */
class Image {
 public:
  /**
     * @brief Constructor for creating an image with the specified width and height.
     *
     * @param width The width of the image.
     * @param height The height of the image.
     */
  Image(size_t width, size_t height);

  /**
     * @brief Constructor for loading an image from a file.
     *
     * @param path The path to the image file.
     */
  Image(std::filesystem::path path);

  /**
     * @brief Load an image from a file.
     *
     * @param path The path to the image file.
     */
  void load(std::filesystem::path path);

  /**
     * @brief Save the image to a file in PNG format.
     *
     * @param path The path to the output file.
     */
  void save(std::filesystem::path path) const;

  /**
     * @brief Update the OpenGL texture ID associated with the image.
     */
  void updateTextureId();

  /**
     * @brief Get the OpenGL texture ID associated with the image.
     *
     * @return The OpenGL texture ID.
     */
  uint64_t getTextureId() const;

  // Static image objects representing icons
  static Image ICON_CONSOLE_TOTAL;
  static Image ICON_CONSOLE_ERROR;
  static Image ICON_CONSOLE_INFO;
  static Image ICON_CONSOLE_VERBOSE;
  static Image ICON_CONSOLE_SUCCESS;

  static Image ICON_COMPONENT_TRANSFORM;
  static Image ICON_COMPONENT_SCRIPT;
  static Image ICON_COMPONENT_SHAPE;

  static Image ICON_FILES_FOLDER;
  static Image ICON_FILES_PYTHON;
  static Image ICON_FILES_IMAGE;
  static Image ICON_FILES_TEXT;

  static Image ICON_BUTTON_PLAY;
  static Image ICON_BUTTON_STOP;

 private:
  uint8_t* _data;  ///< The image pixel data.

  int _width;       ///< The width of the image.
  int _height;      ///< The height of the image.
  int _components;  ///< The number of color components.

  bool
      _wasAssignedTextureId;  ///< Flag indicating if the texture ID has been assigned.
  uint64_t _textureId;  ///< The OpenGL texture ID.
};

}  // namespace shkyera
