add_library(
    Systems
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/GizmoSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ParticleSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AudioSystem.cpp
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

    miniaudio
    glad
    glm::glm
    InputManager
    Rendering
    ECS
    Utils
)
