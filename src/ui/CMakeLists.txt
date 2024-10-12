add_library(
    UI

    ${CMAKE_CURRENT_LIST_DIR}/UI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widget.cpp

    ${CMAKE_CURRENT_LIST_DIR}/widgets/PreviewWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/widgets/ObjectsWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/widgets/ConsoleWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/widgets/PropertiesWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/widgets/FilesystemWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/widgets/SceneWidget.cpp
)

target_include_directories(
    UI
    PRIVATE 
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
)

target_link_libraries(
    UI
    PRIVATE 
    
    Game
    ECS

    imgui
    glfw 
    glad
)