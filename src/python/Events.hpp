#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "renderer/Renderer.hpp"

namespace py = pybind11;

namespace shkyera::Python {

enum Event : int {
    LOG_INFO,
    LOG_ERROR,
    LOG_SUCCESS,
    LOG_VERBOSE,

    DRAW_LINE,

    TOTAL_EVENTS
};

template <typename T> std::vector<T> parsePayload(py::list payload);
template <Event event> void processEvent(py::list payload);

void setRenderer(std::shared_ptr<Renderer> renderer);

} // namespace shkyera::Python
