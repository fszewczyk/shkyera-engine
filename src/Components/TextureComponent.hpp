#pragma once

#include <string>
#include <iostream>

#include <Components/BaseComponent.hpp>

namespace shkyera {

class TextureComponent : public BaseComponent<TextureComponent> {
public:
    TextureComponent() = default;

    std::string& getPath() {
        return _path;
    }

    const std::string& getPath() const {
        return _path;
    }

    void updateImpl() {
        std::cerr << _path << std::endl;
    }

private:
    std::string _path;
};

} // namespace shkyera
