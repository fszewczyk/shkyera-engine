add_library(
    JobSystem
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/ThreadWorker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JobSystem.cpp
)

target_include_directories(
    JobSystem    
    PUBLIC
    
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    JobSystem
    PUBLIC

    Common
)
