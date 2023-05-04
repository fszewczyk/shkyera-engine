#pragma once

#include <functional>

#include "core/Filesystem.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

class FilesystemWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

    void setDirectory(std::filesystem::path path);

  private:
    void drawDirectoryTree(const std::shared_ptr<Directory> directory);
    void drawDirectoryContents(const std::shared_ptr<Directory> directory);

    void drawDirectory(const std::shared_ptr<Directory> directory);
    void drawFile(const std::shared_ptr<File> file);
    void drawIconName(const std::string name) const;

    void handleRightMouseClick();
    void drawCreateMenu() const;

    bool askString(const std::string title, std::string &userInput);

    ImTextureID getTextureOfFile(const std::shared_ptr<File> file) const;
    static std::string getDisplayableName(std::string name, size_t maxCharactersInLine = 12);

    static constexpr float CONTENTS_ICON_SIZE = 64;

    std::shared_ptr<Directory> _currentDirectory;
    std::shared_ptr<Directory> _projectDirectory;

    std::vector<std::shared_ptr<Directory>> _directoriesToDisplay;
    std::vector<std::shared_ptr<File>> _filesToDisplay;

    bool _hoveredIcon;

    static std::string DEFAULT_FOLDER_NAME;
    static std::string DEFAULT_FILE_NAME;
};

} // namespace shkyera
