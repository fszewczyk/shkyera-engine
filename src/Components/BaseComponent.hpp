#pragma once

namespace shkyera {

template <typename Derived>
class BaseComponent {
public:
    void update() {
        static_cast<Derived*>(this)->updateImpl();
    }

    virtual ~BaseComponent() = default;
};

}