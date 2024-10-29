add_library(
    Math
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Box.cpp
)

target_include_directories(
    Math    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    Math
    PUBLIC 

    glm::glm
)
