#pragma once

#include <fmt/core.h>
#include <exception>
#include <iostream>

#define SHKYERA_ASSERT(predicate, ...)                      \
    if (!(predicate)) {                                     \
        std::cerr << "Assert Failed: "                      \
                  << __FILE__ << ":" << __LINE__            \
                  << " due to: " << #predicate              \
                  << ", with message: "                     \
                  << fmt::format(__VA_ARGS__) << std::endl; \
        std::terminate();                                   \
    }
