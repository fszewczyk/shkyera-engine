#pragma once

#include <Common/TypeInfo.hpp>
#include <Common/Types.hpp>

namespace shkyera {

/**
 * @brief Base component for implementing update functionality.
 *
 * Serves as a base for components utilizing CRTP to invoke derived class update implementations.
 *
 * @tparam Derived Type of the derived class inheriting from BaseComponent.
 */
template <typename Derived, RuntimeMode Mode = RuntimeMode::PRODUCTION>
class BaseComponent : public TypeInfo<Derived> {
 public:
  /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures derived classes are destructed correctly.
     */
  virtual ~BaseComponent() = default;
};

}  // namespace shkyera
