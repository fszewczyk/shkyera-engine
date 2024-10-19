#pragma once

#include <AssetManager/Filesystem.hpp>

namespace shkyera {

class FileSelector {
    public:
        FileSelector(const std::string& title);

        void setFile(const File& file);
        const std::optional<File>& getFile() const;
        void setUpdateCallback(std::function<void(const File& file)> callback);
        void setClearCallback(std::function<void()> callback);

        void draw();

    private:
        std::string _title;
        std::optional<File> _file;
        std::function<void(const File& file)> _updateCallback;
        std::function<void()> _clearCallback;
};

}
