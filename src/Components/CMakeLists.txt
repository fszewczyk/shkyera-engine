add_library(
    Components
    INTERFACE

    ${CMAKE_CURRENT_LIST_DIR}/AssetComponents/AssetComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AssetComponents/DirectoryComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AssetComponents/SelectedAssetComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/BaseComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CameraComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NameComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TransformComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/GizmoHandleComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ParticleEmitterComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ModelComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AudioSourceComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/WireframeComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SpotLightComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/PointLightComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DirectionalLightComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/PostProcessingVolumeComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AmbientLightComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/BoxColliderComponent.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SkyboxComponent.hpp
)

target_include_directories(
    Components    
    INTERFACE
    
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include
    ${CMAKE_SOURCE_DIR}/extern/glm/include
)

target_link_libraries(
    Components 
    INTERFACE

    Rendering
    Common
    Math
    JobSystem

    glad
    glm
)

set_target_properties(Components PROPERTIES LINKER_LANGUAGE CXX)
