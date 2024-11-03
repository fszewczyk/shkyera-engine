#pragma once

#include <filesystem>

#include <AssetManager/Asset.hpp>

namespace shkyera {

/**
 * @class Image
 * @brief A class for working with images and textures.
 */
class Image : public Asset {
  public:
    /**
     * @brief Constructor for loading an image from a file.
     *
     * @param path The path to the image file.
     */
    Image(const std::string& path);

    /**
     * @brief Load an image from a file.
     *
     * @param path The path to the image file.
     */
    void load(const std::string& path);

    /**
     * @brief Save the image to a file in PNG format.
     *
     * @param path The path to the output file.
     */
    void save(const std::string& path) const;

    uint8_t const* getData() const;
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

    static std::string ICON_CONSOLE_TOTAL;
    static std::string ICON_CONSOLE_ERROR;
    static std::string ICON_CONSOLE_INFO;
    static std::string ICON_CONSOLE_VERBOSE;
    static std::string ICON_CONSOLE_SUCCESS;

    static std::string ICON_COMPONENT_TRANSFORM;
    static std::string ICON_COMPONENT_SCRIPT;
    static std::string ICON_COMPONENT_SHAPE;

    static std::string ICON_FILES_FOLDER;
    static std::string ICON_FILES_PYTHON;
    static std::string ICON_FILES_IMAGE;
    static std::string ICON_FILES_TEXT;

    static std::string ICON_BUTTON_PLAY;
    static std::string ICON_BUTTON_STOP;

  private:
    uint8_t *_data; ///< The image pixel data.

    int _width;      ///< The width of the image.
    int _height;     ///< The height of the image.
    int _components; ///< The number of color components.
};

} // namespace shkyera
