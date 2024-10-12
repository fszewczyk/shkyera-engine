/**
 * @file FilesystemWidget.hpp
 *
 * @brief Contains the declaration of the `FilesystemWidget` class, a user interface widget for interacting with the
 * filesystem.
 */

#pragma once

#include <functional>

#include <AssetManager/Filesystem.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for interacting with the filesystem.
 *
 * The `FilesystemWidget` class provides a graphical user interface for navigating and interacting with the filesystem,
 * displaying directories, files, and enabling file-related actions.
 */
class FilesystemWidget : public Widget {
  public:
    using Widget::Widget;

    /**
     * @brief Implementation of the abstract `draw` method to render the filesystem widget.
     */
    virtual void draw() override;

    /**
     * @brief Set the current working directory to display its contents.
     *
     * @param path The path to the directory to set as the current working directory.
     */
    void setDirectory(std::filesystem::path path);

  private:
    /**
     * @brief Draw the directory tree, starting from the specified directory.
     *
     * @param directory The root directory to draw.
     */
    void drawDirectoryTree(const std::shared_ptr<Directory> directory);

    /**
     * @brief Draw the contents of the specified directory.
     *
     * @param directory The directory whose contents to draw.
     */
    void drawDirectoryContents(const std::shared_ptr<Directory> directory);

    /**
     * @brief Draw a directory icon and name.
     *
     * @param directory The directory to draw.
     */
    void drawDirectory(const std::shared_ptr<Directory> directory);

    /**
     * @brief Draw a file icon and name.
     *
     * @param file The file to draw.
     */
    void drawFile(const std::shared_ptr<File> file);

    /**
     * @brief Draw an icon and name within the UI.
     *
     * @param name The name to display.
     */
    void drawIconName(const std::string name) const;

    /**
     * @brief Handle a right mouse click within the UI.
     */
    void handleRightMouseClick();

    /**
     * @brief Draw a context menu for creating new files or directories.
     */
    void drawCreateMenu() const;

    /**
     * @brief Get the texture ID associated with a specific file for rendering an icon.
     *
     * @param file The file for which to retrieve the texture ID.
     * @return The texture ID for rendering the file's icon.
     */
    ImTextureID getTextureOfFile(const std::shared_ptr<File> file) const;

    /**
     * @brief Get a displayable name for rendering within the UI, ensuring it doesn't exceed a maximum line length.
     *
     * @param name The original name.
     * @param maxCharactersInLine The maximum number of characters in a line.
     * @return The displayable name.
     */
    static std::string getDisplayableName(std::string name, size_t maxCharactersInLine = 12);

    std::shared_ptr<Directory> _currentDirectory;                  ///< The current working directory to display.
    std::vector<std::shared_ptr<Directory>> _directoriesToDisplay; ///< Directories to display in the UI.
    std::vector<std::shared_ptr<File>> _filesToDisplay;            ///< Files to display in the UI.

    bool _hoveredIcon; ///< A flag indicating whether an icon is currently hovered.

    static constexpr float CONTENTS_ICON_SIZE = 64; ///< The size of icons representing directory contents.
    static std::string DEFAULT_FOLDER_NAME;         ///< The default folder name.
    static std::string DEFAULT_FILE_NAME;           ///< The default file name.
};

} // namespace shkyera
