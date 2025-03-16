add_library(
    Common
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Clock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Profiler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Random.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serialization.hpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
    message(STATUS "Compiling with ARM NEON SIMD")
    list(APPEND COMMON_SOURCES ${CMAKE_CURRENT_LIST_DIR}/SIMDArm.cpp)
else()
    message(STATUS "Compiling with SISD fallback (scalar implementation)")
    list(APPEND COMMON_SOURCES ${CMAKE_CURRENT_LIST_DIR}/SIMDSISD.cpp)
endif()

target_sources(Common PRIVATE ${COMMON_SOURCES})

# Include directories
target_include_directories(
    Common    
    PUBLIC    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glm/include
)

# Link libraries
target_link_libraries(
    Common 
    PUBLIC
    glm
    cereal
)

# Ensure C++ is used
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
