add_library(Runtime STATIC

                ${CMAKE_CURRENT_LIST_DIR} /
            Runtime.cpp)

    target_include_directories(Runtime PUBLIC

                                   ${CMAKE_SOURCE_DIR} /
                               src)
