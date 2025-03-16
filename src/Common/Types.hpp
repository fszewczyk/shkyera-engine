#pragma once

#include <filesystem>

namespace shkyera {

enum class RuntimeMode {
    DEVELOPMENT,
    PRODUCTION
};

template <typename T, typename... Args>
concept PathConstructible = requires(const std::filesystem::path& p, Args&&... args) {
    T{p, std::forward<decltype(args)>(args)...};
};

struct GlobalPathHash {
    std::size_t operator()(const std::filesystem::path& p) const {
        std::string absolutePath = std::filesystem::absolute(p);
        return std::hash<std::string>()(absolutePath);
    }
};

struct GlobalPathEqual {
    bool operator()(const std::filesystem::path& lhs, const std::filesystem::path& rhs) const {
        const auto lhsAbsolute = std::filesystem::absolute(lhs);
        const auto rhsAbsolute = std::filesystem::absolute(rhs);
        return lhsAbsolute == rhsAbsolute;
    }
};

}  // namespace shkyera