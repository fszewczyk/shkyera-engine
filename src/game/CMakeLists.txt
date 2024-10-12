add_library(
    Game
    
    ${CMAKE_CURRENT_LIST_DIR}/Game.cpp
)

target_include_directories(
    Game    
    PRIVATE
    
    ${CMAKE_SOURCE_DIR}/src
)
