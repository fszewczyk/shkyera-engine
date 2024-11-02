add_library(
    Rendering
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/ShaderProgram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FrameBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CubeMap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp
)

target_include_directories(
    Rendering    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    Rendering
    PUBLIC 

    glad
    glm::glm
    AssetManager
)
