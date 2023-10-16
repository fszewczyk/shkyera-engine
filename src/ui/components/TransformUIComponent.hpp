/**
 * @file TransformUIComponent.hpp
 *
 * @brief Contains the declaration of the `TransformUIComponent` class, representing a user interface component for
 * transforming objects.
 */

#pragma once

#include "ui/UIComponent.hpp"

namespace shkyera {

/**
 * @brief A class representing a user interface component for manipulating object transformations.
 *
 * The `TransformUIComponent` class is designed to provide a user interface for manipulating the transformation
 * of objects, such as their position, rotation, and scale.
 */
class TransformUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    /**
     * @brief Implementation of the abstract `draw` method to render the UI for this component.
     */
    virtual void draw() override;
};

} // namespace shkyera
