add_library(
    UI
    STATIC

    ${CMAKE_CURRENT_LIST_DIR}/UI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widget.cpp

    ${CMAKE_CURRENT_LIST_DIR}/Common/Style.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/ColorSelector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/BooleanSelector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/Utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/FloatSlider.cpp

    ${CMAKE_CURRENT_LIST_DIR}/Components/TransformComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/ModelComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/BillboardComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/CameraComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/PointLightComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/SpotLightComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/DirectionalLightComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/ParticleEmitterComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/AmbientLightComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/WireframeComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/PostProcessingVolumeComponentUI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Components/AudioSourceComponentUI.cpp

    ${CMAKE_CURRENT_LIST_DIR}/Widgets/ObjectsWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/ConsoleWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/PropertiesWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/InspectorWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/ProfilerWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/FilesystemWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/SceneWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Widgets/RuntimeWidget.cpp
)

target_include_directories(
    UI
    PUBLIC 
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/imgui
    ${CMAKE_SOURCE_DIR}/extern/glfw/include
    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

target_link_libraries(
    UI
    PUBLIC 
    
    ECS
    Components
    Common
    AssetManager
    InputManager
    Systems
    Utils

    imgui
    glfw 
    glad
)
