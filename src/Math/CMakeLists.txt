add_library(
    Math
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/AABB.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Ray.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp
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
    cereal
)
