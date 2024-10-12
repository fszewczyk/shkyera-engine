add_library(
    ECS
    
    ${CMAKE_CURRENT_LIST_DIR}/EntityProvider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Registry.cpp
)

target_include_directories(
    ECS    
    PRIVATE
    
    ${CMAKE_SOURCE_DIR}/src
)
