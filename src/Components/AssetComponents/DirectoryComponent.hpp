#pragma once

#include <filesystem>

namespace shkyera {

struct DirectoryComponent {
    explicit DirectoryComponent(std::filesystem::path pPath)
        : path(std::move(pPath)) {}

    std::filesystem::path path;
};

}  // namespace shkyera
