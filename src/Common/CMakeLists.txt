add_library(
    Common
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Profiler.cpp
)

target_include_directories(
    Common    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
