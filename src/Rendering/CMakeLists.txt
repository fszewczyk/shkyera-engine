add_library(
    Rendering
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/ShaderProgram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp

    ${CMAKE_CURRENT_LIST_DIR}/FrameBuffers/SceneFrameBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FrameBuffers/DepthAtlasFrameBuffer.cpp
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

target_compile_definitions(
    Rendering
    PUBLIC
    GLFW_INCLUDE_NONE
)
