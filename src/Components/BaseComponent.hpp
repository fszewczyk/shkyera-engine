#pragma once

namespace shkyera {

enum ComponentMode {
    DEVELOPMENT,
    PRODUCTION
};

/**
 * @brief Base component for implementing update functionality.
 *
 * Serves as a base for components utilizing CRTP to invoke derived class update implementations.
 *
 * @tparam Derived Type of the derived class inheriting from BaseComponent.
 */
template <typename Derived, ComponentMode Mode = PRODUCTION>
class BaseComponent {
public:
    /**
     * @brief Calls the derived class's update implementation.
     *
     * Uses static_cast to convert to the derived class type and
     * invokes the updateImpl function.
     */
    void update() {
        static_cast<Derived*>(this)->updateImpl();
    }

    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures derived classes are destructed correctly.
     */
    virtual ~BaseComponent() = default;
};

} // namespace shkyera
