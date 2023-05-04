#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace shkyera {

enum FILE_TYPE { PYTHON, IMAGE, OTHER };

class File {
  public:
    File(std::filesystem::path path);

    std::string getName() const;
    FILE_TYPE getType() const;

  private:
    std::filesystem::path _path;
    FILE_TYPE _type;
};

class Directory {
  public:
    Directory(std::filesystem::path path);

    void update();
    std::string getName() const;
    std::filesystem::path getPath() const;

    std::vector<std::shared_ptr<Directory>> getSubDirectories() const;
    std::vector<std::shared_ptr<File>> getFiles() const;

    void createDirectory(std::string name);
    void createFile(std::string name);

  private:
    std::filesystem::path _path;

    std::vector<std::shared_ptr<File>> _files;
    std::vector<std::shared_ptr<Directory>> _directories;
};

} // namespace shkyera
