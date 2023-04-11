set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/extern)

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/buildGlfw.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/extern/buildImgui.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/extern/buildGlad.cmake)
