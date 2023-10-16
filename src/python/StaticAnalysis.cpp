#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>

#include "python/Interpreter.hpp"
#include "python/StaticAnalysis.hpp"

namespace shkyera::Python {

namespace py = pybind11;

std::unordered_map<std::string, PYTHON_TYPE> _mapPythonTypes = {{"int", INT}, {"float", FLOAT}, {"str", STRING}};

std::vector<std::pair<std::string, PYTHON_TYPE>> getPublicVariables(std::string scriptName) {
    py::scoped_interpreter guard{};

    py::object _script = py::module_::import((MODULE + scriptName).c_str());
    py::object _types = py::module_::import((MODULE + "lib.types").c_str());

    py::object _scriptClass = _script.attr("Object");
    py::list vars = _types.attr("get_types")(_scriptClass);

    std::vector<std::pair<std::string, PYTHON_TYPE>> publicVariables;

    for (py::handle varRef : vars) {
        std::tuple<py::object, py::object> var = varRef.cast<std::tuple<py::object, py::object>>();

        std::string varName = std::get<0>(var).cast<std::string>();
        std::string varType = std::get<1>(var).cast<std::string>();

        PYTHON_TYPE varPythonType = _mapPythonTypes[varType];

        publicVariables.push_back({varName, varPythonType});
    }

    return publicVariables;
}

} // namespace shkyera::Python
