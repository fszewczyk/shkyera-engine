#pragma once

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
    void drawFile(const std::shared_ptr<File> file) const;

    ImTextureID getTextureOfFile(const std::shared_ptr<File> file) const;

    std::shared_ptr<Directory> _currentDirectory;
    std::shared_ptr<Directory> _projectDirectory;

    std::vector<std::shared_ptr<Directory>> _directoriesToDisplay;
    std::vector<std::shared_ptr<File>> _filesToDisplay;

    static constexpr float CONTENTS_ICON_SIZE = 64;
};

} // namespace shkyera
