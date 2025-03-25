#pragma once

#include <string>

#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class NameComponent : public BaseComponent<NameComponent> {
   public:
    NameComponent()
        : _name("Entity") {}
    NameComponent(std::string name)
        : _name(std::move(name)) {}

    void setName(const std::string& name) {
        _name = name;
    }

    std::string& getName() {
        return _name;
    }

    const std::string& getName() const {
        return _name;
    }

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(_name));
    }

   private:
    std::string _name;
};

}  // namespace shkyera
