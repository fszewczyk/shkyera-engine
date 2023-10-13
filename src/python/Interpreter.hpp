#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "renderer/Renderer.hpp"

namespace py = pybind11;

namespace shkyera::Python {

const std::string MODULE = "src.python.shkyera.";

void start();
void stop();
void allowRunning();

bool isRunning();

void setInterpreterRenderer(std::shared_ptr<Renderer> renderer);

} // namespace shkyera::Python
