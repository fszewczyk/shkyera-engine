#pragma once

namespace shkyera::Python {

enum PYTHON_TYPE {
    INT = 0,
    FLOAT = 1,
    STRING = 2,

    TOTAL_TYPES = 3
};

const std::string SCRIPTS_MODULE = "resources.scripts.";

std::vector<std::pair<std::string, PYTHON_TYPE>> getPublicVariables(std::string scriptName);

} // namespace shkyera::Python
