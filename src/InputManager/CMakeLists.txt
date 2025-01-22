add_library(
    InputManager
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/InputManager.cpp
)

target_include_directories(
    InputManager    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    InputManager
    PUBLIC 

    ECS
    Components
    glfw
    glm::glm
)