add_library(
    Utils
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/AssetUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TransformUtils.cpp
)

target_include_directories(
    Utils
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    Utils
    PUBLIC

    ECS
    Components
)
