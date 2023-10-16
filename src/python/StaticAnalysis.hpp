/**
 * @file StaticAnalysis.hpp
 *
 * @brief Contains declarations for Python-related types and functions.
 */

#pragma once

namespace shkyera::Python {

/**
 * @brief Enum representing different types that can be used in Python.
 */
enum PYTHON_TYPE : int {
    INT = 0,    ///< Integer type.
    FLOAT = 1,  ///< Float type.
    STRING = 2, ///< String type.

    TOTAL_TYPES = 3 ///< The total number of supported types.
};

/**
 * @brief Function to get a list of public variables and their types from a Python script.
 *
 * @param scriptName The name of the Python script to inspect.
 * @return A vector of pairs, where each pair contains a variable name and its associated type.
 */
std::vector<std::pair<std::string, PYTHON_TYPE>> getPublicVariables(std::string scriptName);

} // namespace shkyera::Python
