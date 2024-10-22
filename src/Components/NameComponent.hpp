#pragma once

#include <string>

#include <Components/BaseComponent.hpp>

namespace shkyera {

class NameComponent : public BaseComponent<NameComponent> {
public:
    NameComponent() : _name("Entity") {}

    void setName(const std::string &name) 
    {
        _name = name;
    }

    std::string& getName()
    {
        return _name;
    }

    const std::string& getName() const
    {
        return _name;
    }

private:
    std::string _name;
};

} // namespace shkyera
