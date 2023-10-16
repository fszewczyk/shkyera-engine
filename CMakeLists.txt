cmake_minimum_required(VERSION 3.14)

project(ShkyeraEngine)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIE -no-pie")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/extern.cmake)

add_library(
    ui
    src/ui/UI.cpp
    src/ui/Widget.cpp
    src/ui/UIComponent.cpp

    src/ui/widgets/PreviewWidget.cpp
    src/ui/widgets/ObjectsWidget.cpp
    src/ui/widgets/ConsoleWidget.cpp
    src/ui/widgets/PropertiesWidget.cpp
    src/ui/widgets/FilesystemWidget.cpp
    src/ui/widgets/SceneWidget.cpp

    src/ui/components/TransformUIComponent.cpp
    src/ui/components/ScriptUIComponent.cpp
)

add_library(
    core
    src/core/Entity.cpp
    src/core/Image.cpp
    src/core/Filesystem.cpp
    src/core/UUID.cpp
)


add_library(
    game
    src/game/Game.cpp
    src/game/GameObject.cpp
    src/game/Component.cpp

    src/game/components/ScriptComponent.cpp
)

add_library(
    renderer
    src/renderer/Renderer.cpp
)

add_library(
    python
    src/python/Interpreter.cpp
    src/python/Events.cpp
    src/python/StaticAnalysis.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/stbi
)
target_include_directories(game PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm
)
target_include_directories(renderer PRIVATE 
    src
)
target_include_directories(python PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/pybind11/include
)

target_link_libraries(ShkyeraEngine ui)
target_link_libraries(ui 
    imgui
    glfw 
    glad
    game
    python
    renderer
)
target_link_libraries(game  
    core
    python
)
target_link_libraries(renderer  
    glfw 
    glad
    glm::glm
)
target_link_libraries(core 
    stb_image
    glfw 
    glad
    glm::glm
)
target_link_libraries(python 
    ${PYTHON_LIBRARIES}
    pybind11::embed
    renderer
    ui
)


