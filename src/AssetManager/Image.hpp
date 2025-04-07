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
     * @brief Constructor for loading an image from a file.
     *
     * @param path The path to the image file.
     */
    Image(const std::filesystem::path& path);

    Image(const Image& other) = delete;
    Image& operator=(const Image& other) = delete;

    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

    /**
     * @brief Load an image from a file.
     *
     * @param path The path to the image file.
     */
    void load(const std::filesystem::path& path);

    /**
     * @brief Save the image to a file in PNG format.
     *
     * @param path The path to the output file.
     */
    void save(const std::filesystem::path& path) const;

    uint8_t const* getData() const;
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

    static std::filesystem::path ICON_CONSOLE_TOTAL;
    static std::filesystem::path ICON_CONSOLE_ERROR;
    static std::filesystem::path ICON_CONSOLE_INFO;
    static std::filesystem::path ICON_CONSOLE_VERBOSE;
    static std::filesystem::path ICON_CONSOLE_SUCCESS;

    static std::filesystem::path ICON_COMPONENT_TRANSFORM;
    static std::filesystem::path ICON_COMPONENT_SCRIPT;
    static std::filesystem::path ICON_COMPONENT_SHAPE;

    static std::filesystem::path ICON_FILES_FOLDER;
    static std::filesystem::path ICON_FILES_PYTHON;
    static std::filesystem::path ICON_FILES_IMAGE;
    static std::filesystem::path ICON_FILES_TEXT;

    static std::filesystem::path ICON_BUTTON_PLAY;
    static std::filesystem::path ICON_BUTTON_STOP;

   private:
    uint8_t* _data;  ///< The image pixel data.

    int _width;       ///< The width of the image.
    int _height;      ///< The height of the image.
    int _components;  ///< The number of color components.
};

}  // namespace shkyera
