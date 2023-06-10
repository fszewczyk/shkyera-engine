#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace shkyera::Python {

enum Event : int {
    LOG_INFO,
    LOG_ERROR,
    LOG_SUCCESS,
    LOG_VERBOSE,

    DRAW_LINE,
    DRAW_CIRCLE,

    TOTAL_EVENTS
};

template <typename T> std::vector<T> parsePayload(py::list payload);
template <Event event> void processEvent(py::list payload);

} // namespace shkyera::Python

#include "python/Events.inl"
