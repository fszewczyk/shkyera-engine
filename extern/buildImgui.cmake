set(IMGUI_SOURCE
    extern/imgui/imgui.cpp
    extern/imgui/imgui_demo.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
    extern/imgui/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui STATIC
    ${IMGUI_SOURCE}
)

target_include_directories(imgui PUBLIC "extern/imgui")
target_link_libraries(imgui PRIVATE glfw)