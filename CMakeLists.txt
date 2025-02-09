cmake_minimum_required(VERSION 3.14)

project(ShkyeraEngine)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -no-pie")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package (Python3 COMPONENTS Interpreter Development)
include_directories(${PYTHON_INCLUDE_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/extern.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/AssetManager)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Components)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/ECS)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Rendering)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Runtime)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/UI)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/InputManager)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Math)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Systems)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Common)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/Utils)

add_executable(ShkyeraEngine src/main.cpp)

target_include_directories(
    ShkyeraEngine
    PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    ShkyeraEngine
    PUBLIC 

    UI
    Runtime
)
