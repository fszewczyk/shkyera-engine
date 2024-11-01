add_library(
    Common
    INTERFACE

    ${CMAKE_CURRENT_LIST_DIR}/Types.hpp
)

set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
