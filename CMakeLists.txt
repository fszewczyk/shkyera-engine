cmake_minimum_required(VERSION 3.14)

project(ShkyeraEngine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIE -no-pie")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/extern.cmake)

add_library(
    ui
    src/ui/UI.cpp
    src/ui/Widget.cpp
    src/ui/widgets/PreviewWidget.cpp
)

add_library(
    core
    src/core/Entity.cpp
    src/core/Entity.hpp
    src/core/Grid.cpp
)

add_library(
    components
    src/components/Component.cpp
    src/components/FluidGrid.cpp
)

add_library(
    game
    src/game/Game.cpp
    src/game/GameObject.cpp
)

add_executable(ShkyeraEngine src/main.cpp)


target_include_directories(ShkyeraEngine PRIVATE src)
target_include_directories(ui PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
)
target_include_directories(core PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
)
target_include_directories(components PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
)
target_include_directories(game PRIVATE src)

message(${CMAKE_CURRENT_SOURCE_DIR}/extern/glm)

target_link_libraries(ShkyeraEngine  ui)
target_link_libraries(ui  
    imgui
    glfw 
    glad
)
target_link_libraries(components  
    core
    glm::glm
)
target_link_libraries(game  
    components
)
