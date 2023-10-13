#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "renderer/Renderer.hpp"

namespace py = pybind11;

namespace shkyera::Python {

/**
 * @brief The Python module prefix for shkyera.
 */
const std::string MODULE = "src.python.shkyera.";

/**
 * @brief Start the Python interpreter and initialize the Python environment.
 */
void start();

/**
 * @brief Stop the Python interpreter and finalize the Python environment.
 */
void stop();

/**
 * @brief Allow the Python environment to run.
 */
void allowRunning();

/**
 * @brief Check if the Python environment is running.
 *
 * @return True if the Python environment is running, false otherwise.
 */
bool isRunning();

/**
 * @brief Set the renderer to use for the Python interpreter.
 *
 * @param renderer A shared pointer to a Renderer object.
 */
void setInterpreterRenderer(std::shared_ptr<Renderer> renderer);

} // namespace shkyera::Python
