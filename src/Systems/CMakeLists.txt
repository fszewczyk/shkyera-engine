add_library(
    Systems
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/CameraMovementSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RenderingSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectSelectionSystem.cpp
)

target_include_directories(
    Systems    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    Systems
    PUBLIC 

    glad
    glm::glm
    InputManager
    Rendering
    ECS
)
