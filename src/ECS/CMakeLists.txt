add_library(
    ECS
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/EntityProvider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Registry.cpp
)

target_include_directories(
    ECS    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    ECS 
    PUBLIC 

    Common
)
