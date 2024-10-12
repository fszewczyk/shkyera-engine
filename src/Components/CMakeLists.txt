add_library(Components INTERFACE

                ${CMAKE_CURRENT_LIST_DIR} /
            BaseComponent.hpp ${CMAKE_CURRENT_LIST_DIR} / NameComponent.hpp)

    target_include_directories(Components INTERFACE

                                   ${CMAKE_SOURCE_DIR} /
                               src)

        set_target_properties(Components PROPERTIES LINKER_LANGUAGE CXX)
