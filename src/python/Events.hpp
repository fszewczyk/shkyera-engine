/**
 * @file Events.hpp
 *
 * @brief Contains the declarations for Python interaction functions and event handling.
 */

#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "renderer/Renderer.hpp"

namespace py = pybind11;

namespace shkyera::Python {

/**
 * @brief Enum representing different types of events that can be processed in Python.
 */
enum Event : int {
    LOG_INFO,    ///< Log an informational message.
    LOG_ERROR,   ///< Log an error message.
    LOG_SUCCESS, ///< Log a success message.
    LOG_VERBOSE, ///< Log a verbose message.

    DRAW_LINE,      ///< Draw a line using the renderer.
    DRAW_CIRCLE,    ///< Draw a circle using the renderer.
    DRAW_RECTANGLE, ///< Draw a rectangle using the renderer
    DRAW_CLEAR,     ///< Clear the renderer.

    TOTAL_EVENTS ///< The total number of events.
};

/**
 * @brief Template function to parse a payload from a Python list into a C++ vector.
 *
 * @param payload A Python list containing data.
 * @return A C++ vector of the parsed data.
 */
template <typename T> std::vector<T> parsePayload(py::list payload);

/**
 * @brief Function to process an event of a specific type.
 *
 * @param event The type of event to process.
 * @param payload A Python list containing event-specific data.
 */
template <Event event> void processEvent(py::list payload);

/**
 * @brief Set the renderer to be used for drawing in Python.
 *
 * @param renderer A shared pointer to the renderer.
 */
void setRenderer(std::shared_ptr<Renderer> renderer);

} // namespace shkyera::Python
