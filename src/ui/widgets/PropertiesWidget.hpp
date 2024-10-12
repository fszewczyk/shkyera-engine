/**
 * @file PropertiesWidget.hpp
 *
 * @brief Contains the declaration of the `PropertiesWidget` class, a user interface widget for managing game object
 * properties.
 */

#pragma once

#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for managing game object properties.
 *
 * The `PropertiesWidget` class provides a graphical user interface for viewing and managing the properties and
 * components of a selected game object.
 */
class PropertiesWidget : public Widget {
  public:
    using Widget::Widget;

    /**
     * @brief Implementation of the abstract `draw` method to render the properties widget.
     */
    virtual void draw() override;
};

} // namespace shkyera
