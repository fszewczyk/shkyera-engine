add_library(
    Components
    INTERFACE

    ${CMAKE_CURRENT_LIST_DIR}/BaseComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CameraComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NameComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TransformComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MeshComponent.hpp
)

target_include_directories(
    Components    
    INTERFACE
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include
    ${CMAKE_SOURCE_DIR}/extern/glm/include
)

target_link_libraries(
    Components 
    INTERFACE

    Rendering
    glad
    glm
)

set_target_properties(Components PROPERTIES LINKER_LANGUAGE CXX)
