option(SKIP_PERFORMANCE_COMPARISON "" ON)
option(BUILD_DOC "" OFF)
option(BUILD_SANDBOX "" OFF)
add_subdirectory(extern/cereal)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(cereal INTERFACE -Wno-ambiguous-reversed-operator)
endif()

