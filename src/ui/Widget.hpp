/**
 * @file Widget.hpp
 *
 * @brief Contains the declaration of the `Widget` class, an abstract base class for GUI widgets.
 */

#pragma once

#include <cmath>
#include <string>

#include <GLFW/glfw3.h>

namespace shkyera {

/**
 * @brief An abstract base class representing a GUI widget.
 *
 * The `Widget` class serves as a base class for various graphical user interface (GUI) widgets
 * and defines a common interface for drawing them.
 */
class Widget {
  public:
    /**
     * @brief Constructor to create a widget with a specified name.
     *
     * @param name The name of the widget.
     */
    Widget(std::string name);

    /**
     * @brief Default virtual destructor
     */
    virtual ~Widget() = default;

    /**
     * @brief Abstract method to draw the widget.
     *
     * This method must be implemented by derived classes to render the widget on the screen.
     */
    virtual void draw() = 0;

    std::string _name; ///< The name of the widget.
};

} // namespace shkyera
