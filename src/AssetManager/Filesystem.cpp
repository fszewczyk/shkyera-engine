#include <fstream>
#include <iostream>

#include <AssetManager/Filesystem.hpp>

namespace shkyera {

File::File(std::filesystem::path path) : _path(path) {
  std::string extension = path.extension().string();

  if (extension == ".py")
    _type = PYTHON;
  else if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" ||
           extension == ".bmp")
    _type = IMAGE;
  else
    _type = OTHER;
}

std::filesystem::path File::getPath() const {
  return _path;
}
std::string File::getName() const {
  return _path.stem().string() + _path.extension().string();
}
std::string File::getNameWithoutExtension() const {
  return _path.stem().string();
}
FILE_TYPE File::getType() const {
  return _type;
}

Directory::Directory(std::filesystem::path path) : _path(path) {
  update();
}

void Directory::update() {
  _directories.clear();
  _files.clear();
  if (_rootDirectory && _path == _rootDirectory->getPath())
    _mapFiles.clear();

  for (const auto& subPath : std::filesystem::directory_iterator(_path)) {
    if (std::filesystem::is_directory(subPath)) {
      _directories.push_back(std::make_shared<Directory>(subPath));
    } else {
      if (subPath.path().filename().string().at(0) != '.') {
        _files.push_back(std::make_shared<File>(subPath));
        _mapFiles[subPath.path()] = _files.back();
      }
    }
  }
}

std::string Directory::getName() const {
  return _path.stem().string();
}
std::filesystem::path Directory::getPath() const {
  return _path;
}
std::vector<std::shared_ptr<Directory>> Directory::getSubDirectories() const {
  return _directories;
}
std::vector<std::shared_ptr<File>> Directory::getFiles() const {
  return _files;
}

void Directory::createDirectory(std::string name) {
  std::filesystem::path newPath = _path / name;

  if (std::filesystem::exists(newPath))
    throw std::invalid_argument("There is already a folder with this name.");

  std::filesystem::create_directory(newPath);
  update();
}

void Directory::createFile(std::string name) {
  std::filesystem::path newPath = _path / name;

  if (std::filesystem::exists(newPath))
    throw std::invalid_argument("There is already a file with this name.");

  std::ofstream{newPath.c_str()};
  update();
}

void Directory::setRoot(std::shared_ptr<Directory> root) {
  _rootDirectory = root;
}
std::shared_ptr<Directory> Directory::getRoot() {
  return _rootDirectory;
}

std::shared_ptr<File> Directory::getFile(std::filesystem::path path) {
  return _mapFiles.at(path);
}

std::shared_ptr<Directory> Directory::_rootDirectory = nullptr;
std::map<std::filesystem::path, std::shared_ptr<File>> Directory::_mapFiles =
    {};

}  // namespace shkyera
