#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace shkyera {

/**
 * @enum FILE_TYPE
 * @brief An enumeration representing file types.
 */
enum FILE_TYPE { PYTHON, IMAGE, OTHER };

/**
 * @class File
 * @brief A class representing a file in the filesystem.
 */
class File {
  public:
    /**
     * @brief Constructor for a File object.
     *
     * @param path The path to the file.
     */
    File(std::filesystem::path path);

    /**
     * @brief Get the path to the file.
     *
     * @return The path to the file.
     */
    std::filesystem::path getPath() const;

    /**
     * @brief Get the name of the file, including the extension.
     *
     * @return The name of the file.
     */
    std::string getName() const;

    /**
     * @brief Get the type of the file.
     *
     * @return The type of the file (e.g., PYTHON, IMAGE, OTHER).
     */
    FILE_TYPE getType() const;

  private:
    std::filesystem::path _path; ///< The path to the file.
    FILE_TYPE _type;             ///< The type of the file.
};

/**
 * @class Directory
 * @brief A class representing a directory in the filesystem.
 */
class Directory {
  public:
    /**
     * @brief Constructor for a Directory object.
     *
     * @param path The path to the directory.
     */
    Directory(std::filesystem::path path);

    /**
     * @brief Update the content of the directory by listing subdirectories and files.
     */
    void update();

    /**
     * @brief Get the name of the directory.
     *
     * @return The name of the directory.
     */
    std::string getName() const;

    /**
     * @brief Get the path to the directory.
     *
     * @return The path to the directory.
     */
    std::filesystem::path getPath() const;

    /**
     * @brief Get a list of subdirectories within this directory.
     *
     * @return A vector of shared pointers to subdirectories.
     */
    std::vector<std::shared_ptr<Directory>> getSubDirectories() const;

    /**
     * @brief Get a list of files within this directory.
     *
     * @return A vector of shared pointers to files.
     */
    std::vector<std::shared_ptr<File>> getFiles() const;

    /**
     * @brief Create a new subdirectory within this directory.
     *
     * @param name The name of the new subdirectory.
     */
    void createDirectory(std::string name);

    /**
     * @brief Create a new file within this directory.
     *
     * @param name The name of the new file.
     */
    void createFile(std::string name);

    /**
     * @brief Set the root directory of the filesystem.
     *
     * @param root The shared pointer to the root directory.
     */
    static void setRoot(std::shared_ptr<Directory> root);

    /**
     * @brief Get the root directory of the filesystem.
     *
     * @return The shared pointer to the root directory.
     */
    static std::shared_ptr<Directory> getRoot();

    /**
     * @brief Get a file by its path from the filesystem.
     *
     * @param path The path to the file.
     * @return The shared pointer to the file.
     */
    static std::shared_ptr<File> getFile(std::filesystem::path path);

  private:
    std::filesystem::path _path;                          ///< The path to the directory.
    std::vector<std::shared_ptr<File>> _files;            ///< List of files in the directory.
    std::vector<std::shared_ptr<Directory>> _directories; ///< List of subdirectories in the directory.

    static std::shared_ptr<Directory> _rootDirectory;                        ///< The root directory of the filesystem.
    static std::map<std::filesystem::path, std::shared_ptr<File>> _mapFiles; ///< Map of file paths to file objects.
};

} // namespace shkyera
