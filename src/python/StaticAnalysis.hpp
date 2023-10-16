#pragma once

namespace shkyera::Python {

enum PYTHON_TYPE : int {
    INT = 0,
    FLOAT = 1,
    STRING = 2,

    TOTAL_TYPES = 3
};

std::vector<std::pair<std::string, PYTHON_TYPE>> getPublicVariables(std::string scriptName);

} // namespace shkyera::Python
