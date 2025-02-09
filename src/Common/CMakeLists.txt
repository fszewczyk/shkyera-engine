add_library(
    Common
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/Clock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Profiler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Random.cpp
)

target_include_directories(
    Common    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glm/include
)


target_link_libraries(
    Common 
    PUBLIC

    glm
)

set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
