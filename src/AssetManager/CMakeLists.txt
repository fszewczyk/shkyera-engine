add_library(
    AssetManager
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Audio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sound.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Mesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Wireframe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CubeMap.cpp
)

target_include_directories(
    AssetManager    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/stbi
    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

target_link_libraries(
    AssetManager
    PUBLIC

    Math
    Utils
    stb_image
    glfw 
    glm
    glad
    miniaudio
    tinyobjloader
)
