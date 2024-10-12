add_library(
    AssetManager

    ${CMAKE_CURRENT_LIST_DIR}/Filesystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Image.cpp
)

target_include_directories(
    AssetManager    
    PRIVATE
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/stbi
)

target_link_libraries(
    AssetManager
    PRIVATE

    stb_image
    glfw 
    glad
    glm::glm
)