add_library(
    AssetManager
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Filesystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Image.cpp
)

target_include_directories(
    AssetManager    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/stbi
)

target_link_libraries(
    AssetManager
    PUBLIC

    stb_image
    glfw 
    glad
    glm::glm
)