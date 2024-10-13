add_library(
    Rendering
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/ShaderProgram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp
)

target_include_directories(
    Rendering    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

target_link_libraries(
    Rendering
    PUBLIC 

    glad
)
