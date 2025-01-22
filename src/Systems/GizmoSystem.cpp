#include <Systems/GizmoSystem.hpp>
#include <Common/Logger.hpp>

#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Mesh.hpp>
#include <InputManager/InputManager.hpp>
#include <Components/NameComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/GizmoHandleComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <ECS/Registry.hpp>

namespace shkyera {

static shkyera::Entity addOverlayModel(std::shared_ptr<shkyera::Registry> registry, 
              const glm::vec3& position, 
              const glm::vec3& color,
              std::shared_ptr<shkyera::Mesh> mesh) {
    auto entity = registry->addEntity();
    registry->addComponent<TransformComponent>(entity);
    registry->getComponent<TransformComponent>(entity).setPosition(position);
    registry->addComponent<OverlayModelComponent>(entity);
    registry->getComponent<OverlayModelComponent>(entity).setMesh(mesh);

    auto material = std::make_shared<Material>();
    material->setDiffuseColor(color);
    registry->getComponent<OverlayModelComponent>(entity).setMaterial(material);
    registry->addComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity);
    registry->getComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity).box = mesh->getBoundingBox();

    return entity;
}

GizmoSystem::GizmoSystem(std::shared_ptr<Registry> registry)
: _registry(std::move(registry))
{
    { // Translation Gizmo
        _translationGizmo = _registry->addEntity();
        _registry->addComponent<TransformComponent>(_translationGizmo);

        auto xPlusLine = addOverlayModel(_registry, {GizmoLineLength, 0, 0}, Red, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(xPlusLine).setScale({GizmoLineLength, GizmoLineWidth, GizmoLineWidth});
        auto yPlusLine = addOverlayModel(_registry, {0, GizmoLineLength, 0},  Green, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(yPlusLine).setScale({GizmoLineWidth, GizmoLineLength, GizmoLineWidth});
        auto zPlusLine = addOverlayModel(_registry, {0, 0, GizmoLineLength}, Blue, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(zPlusLine).setScale({GizmoLineWidth, GizmoLineWidth, GizmoLineLength});

        auto xPlusHandle = addOverlayModel(_registry, {2 * GizmoLineLength, 0, 0}, Red, std::shared_ptr<Mesh>(Mesh::Factory::createCone()));
        _registry->getComponent<TransformComponent>(xPlusHandle).setScale({1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize});
        _registry->getComponent<TransformComponent>(xPlusHandle).setOrientation({0, 0, -M_PI_2});
        _registry->addComponent<GizmoHandleComponent>(xPlusHandle);
        auto yPlusHandle = addOverlayModel(_registry, {0, 2 * GizmoLineLength, 0}, Green, std::shared_ptr<Mesh>(Mesh::Factory::createCone()));
        _registry->getComponent<TransformComponent>(yPlusHandle).setScale({1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize});
        _registry->addComponent<GizmoHandleComponent>(yPlusHandle);
        auto zPlusHandle = addOverlayModel(_registry, {0, 0, 2 * GizmoLineLength}, Blue, std::shared_ptr<Mesh>(Mesh::Factory::createCone()));
        _registry->getComponent<TransformComponent>(zPlusHandle).setScale({1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize});
        _registry->getComponent<TransformComponent>(zPlusHandle).setOrientation({M_PI_2, 0, 0});
        _registry->addComponent<GizmoHandleComponent>(zPlusHandle);

        _registry->getHierarchy().attributeChild(_translationGizmo, xPlusLine);
        _registry->getHierarchy().attributeChild(_translationGizmo, yPlusLine);
        _registry->getHierarchy().attributeChild(_translationGizmo, zPlusLine);
        _registry->getHierarchy().attributeChild(_translationGizmo, xPlusHandle);
        _registry->getHierarchy().attributeChild(_translationGizmo, yPlusHandle);
        _registry->getHierarchy().attributeChild(_translationGizmo, zPlusHandle);
    }

    { // Scale Gizmo
        _scaleGizmo = _registry->addEntity();
        _registry->addComponent<TransformComponent>(_scaleGizmo);

        _scaleGizmoLineX = addOverlayModel(_registry, {GizmoLineLength, 0, 0}, Red, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(_scaleGizmoLineX).setScale({GizmoLineLength, GizmoLineWidth, GizmoLineWidth});
        _scaleGizmoLineY = addOverlayModel(_registry, {0, GizmoLineLength, 0},  Green, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(_scaleGizmoLineY).setScale({GizmoLineWidth, GizmoLineLength, GizmoLineWidth});
        _scaleGizmoLineZ = addOverlayModel(_registry, {0, 0, GizmoLineLength}, Blue, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(_scaleGizmoLineZ).setScale({GizmoLineWidth, GizmoLineWidth, GizmoLineLength});

        auto xPlusHandle = addOverlayModel(_registry, {2 * GizmoLineLength, 0, 0}, Red, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(xPlusHandle).setScale({GizmoHandleSize, GizmoHandleSize, GizmoHandleSize});
        _registry->getComponent<TransformComponent>(xPlusHandle).setOrientation({0, 0, -M_PI_2});
        _registry->addComponent<GizmoHandleComponent>(xPlusHandle, GizmoHandleComponent::Direction::X);
        auto yPlusHandle = addOverlayModel(_registry, {0, 2 * GizmoLineLength, 0}, Green, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(yPlusHandle).setScale({GizmoHandleSize, GizmoHandleSize, GizmoHandleSize});
        _registry->addComponent<GizmoHandleComponent>(yPlusHandle, GizmoHandleComponent::Direction::Y);
        auto zPlusHandle = addOverlayModel(_registry, {0, 0, 2 * GizmoLineLength}, Blue, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(zPlusHandle).setScale({GizmoHandleSize, GizmoHandleSize, GizmoHandleSize});
        _registry->getComponent<TransformComponent>(zPlusHandle).setOrientation({M_PI_2, 0, 0});
        _registry->addComponent<GizmoHandleComponent>(zPlusHandle, GizmoHandleComponent::Direction::Z);
        _scaleUniformHandle = addOverlayModel(_registry, {0, 0, 0}, Grey, std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
        _registry->getComponent<TransformComponent>(_scaleUniformHandle).setScale({1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize});
        _registry->getComponent<TransformComponent>(_scaleUniformHandle).setOrientation({0, 0, 0});
        _registry->addComponent<GizmoHandleComponent>(_scaleUniformHandle, GizmoHandleComponent::Direction::ANY);

        _registry->getHierarchy().attributeChild(_scaleGizmo, _scaleGizmoLineX);
        _registry->getHierarchy().attributeChild(_scaleGizmo, _scaleGizmoLineY);
        _registry->getHierarchy().attributeChild(_scaleGizmo, _scaleGizmoLineZ);
        _registry->getHierarchy().attributeChild(_scaleGizmo, xPlusHandle);
        _registry->getHierarchy().attributeChild(_scaleGizmo, yPlusHandle);
        _registry->getHierarchy().attributeChild(_scaleGizmo, zPlusHandle);
        _registry->getHierarchy().attributeChild(_scaleGizmo, _scaleUniformHandle);
    }

    { // Rotation Gizmo
        _rotationGizmo = _registry->addEntity();
        _registry->addComponent<TransformComponent>(_rotationGizmo);

        auto xHandle = addOverlayModel(_registry, {0, 0, 0}, Red, std::shared_ptr<Mesh>(Mesh::Factory::createTorus(1.7, 1.65, 32, 32)));
        _registry->getComponent<TransformComponent>(xHandle).setScale({GizmoLineLength, GizmoLineLength, GizmoLineLength});
        _registry->getComponent<TransformComponent>(xHandle).setOrientation({0, M_PI_2, 0});
        _registry->addComponent<GizmoHandleComponent>(xHandle, GizmoHandleComponent::Direction::X);
        auto yHandle = addOverlayModel(_registry, {0, 0, 0}, Green, std::shared_ptr<Mesh>(Mesh::Factory::createTorus(1.7, 1.65, 32, 32)));
        _registry->getComponent<TransformComponent>(yHandle).setScale({GizmoLineLength, GizmoLineLength, GizmoLineLength});
        _registry->getComponent<TransformComponent>(yHandle).setOrientation({M_PI_2, 0, 0});
        _registry->addComponent<GizmoHandleComponent>(yHandle, GizmoHandleComponent::Direction::Y);
        auto zHandle = addOverlayModel(_registry, {0, 0, 0}, Blue, std::shared_ptr<Mesh>(Mesh::Factory::createTorus(1.7, 1.65, 32, 32)));
        _registry->getComponent<TransformComponent>(zHandle).setScale({GizmoLineLength, GizmoLineLength, GizmoLineLength});
        _registry->getComponent<TransformComponent>(zHandle).setOrientation({0, 0, 0});
        _registry->addComponent<GizmoHandleComponent>(zHandle, GizmoHandleComponent::Direction::Z);

        _registry->getHierarchy().attributeChild(_rotationGizmo, xHandle);
        _registry->getHierarchy().attributeChild(_rotationGizmo, yHandle);
        _registry->getHierarchy().attributeChild(_rotationGizmo, zHandle);
    }

    InputManager::getInstance().registerMouseButtonDownCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        const auto hoveredHandleOpt = InputManager::getHoveredObject<
            InputManager::CoordinateSystem::SCENE,
            RuntimeMode::DEVELOPMENT,
            GizmoHandleComponent
        >(_registry);

        if(hoveredHandleOpt)
        {
            if(_controlledEntity)
            {
                _originalScale = _registry->getComponent<TransformComponent>(*_controlledEntity).getScale();
                _originalOrientation = _registry->getComponent<TransformComponent>(*_controlledEntity).getOrientation();
                _selectedHandle = hoveredHandleOpt;
                _originalHandlePosition = _registry->getComponent<TransformComponent>(*_selectedHandle).getPosition();
                _originalDisplacement = getSelectedHandleDisplacement();
                _originalPlaneIntersection = *getMouseIntersectionWithHandle();
            }
        }
    });

    InputManager::getInstance().registerMouseButtonUpCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        if(_selectedHandle)
        {
            auto mat = std::make_shared<Material>(_originalColor, 1);
            _registry->getComponent<OverlayModelComponent>(*_selectedHandle).setMaterial(mat);
            _registry->getComponent<TransformComponent>(*_selectedHandle).setPosition(_originalHandlePosition);
            _registry->getComponent<TransformComponent>(*_selectedHandle).setPosition(_originalHandlePosition);
            _selectedHandle.reset();
        }

        _registry->getComponent<TransformComponent>(_scaleUniformHandle).setScale({1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize, 1.5 * GizmoHandleSize});
        _registry->getComponent<TransformComponent>(_scaleGizmoLineX).setScale({GizmoLineLength, GizmoLineWidth, GizmoLineWidth});
        _registry->getComponent<TransformComponent>(_scaleGizmoLineY).setScale({GizmoLineWidth, GizmoLineLength, GizmoLineWidth});
        _registry->getComponent<TransformComponent>(_scaleGizmoLineZ).setScale({GizmoLineWidth, GizmoLineWidth, GizmoLineLength});
    });

    InputManager::getInstance().registerKeyCallback(GLFW_KEY_T, [this](){
        _mode = Mode::TRANSLATION;
    });
    InputManager::getInstance().registerKeyCallback(GLFW_KEY_Y, [this](){
        _mode = Mode::SCALE;
    });
}

GizmoSystem::~GizmoSystem()
{
    _registry->removeEntity(_translationGizmo);
    _registry->removeEntity(_scaleGizmo);
    _registry->removeEntity(_rotationGizmo);
}

void GizmoSystem::update()
{
    selectEntity();
    styleOnHover();

    moveEntity();
    scaleEntity();

    adjustGizmoTransform();
}

void GizmoSystem::selectEntity()
{
    const auto selectedEntities = _registry->getSelectedEntities();
    if(selectedEntities.empty())
    {
        _controlledEntity.reset();
    }
    else
    {
        _controlledEntity = *selectedEntities.begin();
    }
}

void GizmoSystem::styleOnHover()
{
    if(_selectedHandle)
    {
        return;
    }

    const auto hoveredHandleOpt = InputManager::getHoveredObject<
        InputManager::CoordinateSystem::SCENE,
        RuntimeMode::DEVELOPMENT,
        GizmoHandleComponent
    >(_registry);

    const bool hoverMoved = isHoverMoved(hoveredHandleOpt);

    if(hoverMoved && _hoveredHandle)
    {
        auto mat = std::make_shared<Material>(_originalColor, 1);
        _registry->getComponent<OverlayModelComponent>(*_hoveredHandle).setMaterial(mat);
    }

    _hoveredHandle = hoveredHandleOpt;
    if(hoverMoved && _hoveredHandle)
    {
        const auto& hoveredHandle = *_hoveredHandle;
        _originalColor = _registry->getComponent<OverlayModelComponent>(hoveredHandle).getMaterial()->getDiffuseColor();
        
        auto mat = std::make_shared<Material>(_originalColor * 0.55f, 1);
        _registry->getComponent<OverlayModelComponent>(hoveredHandle).setMaterial(mat);
    }
}

bool GizmoSystem::isHoverMoved(std::optional<Entity> currentHover) const
{
    bool result = currentHover.has_value() != _hoveredHandle.has_value();
    if(currentHover.has_value() && _hoveredHandle.has_value())
    {
        result |= *currentHover != *_hoveredHandle;
    }

    return result;
}

void GizmoSystem::moveEntity()
{
    const auto handleWasSelected = _selectedHandle.has_value();
    const auto positionGizmoEnabled = _mode == Mode::TRANSLATION;
    const auto controllingEntity = _controlledEntity.has_value();
    const auto mouseButtonDown = InputManager::getInstance().isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);
    if(!positionGizmoEnabled || !handleWasSelected || !mouseButtonDown || !controllingEntity)
    {
        return;
    }

    const auto moveDisplacement = getSelectedHandleDisplacement() - _originalDisplacement;
    auto globalRotation = glm::mat3{1.0};
    if(const auto parentOpt = _registry->getHierarchy().getParent(*_controlledEntity))
    {
        globalRotation = glm::inverse(glm::mat3{TransformComponent::getGlobalRotationMatrix(*parentOpt, _registry)});
    }
    const auto localMoveDisplacement = globalRotation * moveDisplacement;

    auto& selectedEntityPosition = _registry->getComponent<TransformComponent>(*_controlledEntity).getPosition();
    selectedEntityPosition += localMoveDisplacement;
}

void GizmoSystem::scaleEntity()
{
    const auto handleWasSelected = _selectedHandle.has_value();
    const auto scaleGizmoEnabled = _mode == Mode::SCALE;
    const auto controllingEntity = _controlledEntity.has_value();
    const auto mouseButtonDown = InputManager::getInstance().isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);
    if(!scaleGizmoEnabled || !handleWasSelected || !mouseButtonDown || !controllingEntity)
    {
        return;
    }

    auto& handleTransform = _registry->getComponent<TransformComponent>(*_selectedHandle);
    const auto currentHandlePosition = handleTransform.getPosition();
    handleTransform.setPosition(_originalHandlePosition);
    const auto globalHandleTransformMatrix = TransformComponent::getGlobalTransformMatrix(*_selectedHandle, _registry);
    handleTransform.setPosition(currentHandlePosition);

    const auto handlePosition = glm::vec3{globalHandleTransformMatrix[3]};
    const auto scaleDisplacement = getSelectedHandleDisplacement() - _originalDisplacement;
    const auto displacedHandle = handlePosition + scaleDisplacement;
    const auto& selectedEntityPosition = _registry->getComponent<TransformComponent>(*_controlledEntity).getPosition();
    const auto handleToSelectedEntity = glm::length(handlePosition - selectedEntityPosition);
    const auto displacedHandleToSelectedEntity = glm::length(displacedHandle - selectedEntityPosition);
    const auto scaleFactor = displacedHandleToSelectedEntity / handleToSelectedEntity;
    
    glm::vec3 scale;
    Entity scaleLine;
    switch(_registry->getComponent<GizmoHandleComponent>(*_selectedHandle).direction)
    {
        case GizmoHandleComponent::Direction::X:
            scale = glm::vec3{scaleFactor, 1, 1};
            scaleLine = _scaleGizmoLineX;
            break;
        case GizmoHandleComponent::Direction::Y:
            scale = glm::vec3{1, scaleFactor, 1};
            scaleLine = _scaleGizmoLineY;
            break;
        case GizmoHandleComponent::Direction::Z:
            scale = glm::vec3{1, 1, scaleFactor};
            scaleLine = _scaleGizmoLineZ;
            break;
        default:
            Logger::ERROR("Unsupported Direction of a Scale Gizmo Handle.");
            return;
    }

    _registry->getComponent<TransformComponent>(scaleLine).setScale(glm::vec3(0.0f));
    handleTransform.setPosition(_originalHandlePosition * scale);
    _registry->getComponent<TransformComponent>(*_controlledEntity).setScale(_originalScale * scale);
}

glm::vec3 GizmoSystem::getSelectedHandleDisplacement() const
{
    auto& handleTransform = _registry->getComponent<TransformComponent>(*_selectedHandle);
    const auto currentHandlePosition = handleTransform.getPosition();

    handleTransform.setPosition(_originalHandlePosition);
    const auto globalHandleTransformMatrix = TransformComponent::getGlobalTransformMatrix(*_selectedHandle, _registry);
    handleTransform.setPosition(currentHandlePosition);
    
    const auto handleDirection = glm::vec3{globalHandleTransformMatrix * glm::vec4{0, 1, 0, 0}};
    const auto handlePosition = glm::vec3{globalHandleTransformMatrix[3]};
    const auto handleRayPositive = Ray{ .origin = handlePosition, .direction = handleDirection };
    const auto handleRayNegative = Ray{ .origin = handlePosition, .direction = -handleDirection };

    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());
    const auto& cameraTransformComponent = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto mousePosition = InputManager::getInstance().getRelativeMousePosition(InputManager::CoordinateSystem::SCENE);
    const auto cameraRay = cameraComponent.getRayAt(cameraTransformComponent, mousePosition.x, 1 - mousePosition.y);

    const auto tPositiveRayProjection = handleRayPositive.project(cameraRay);
    const auto tNegativeRayProjection = handleRayNegative.project(cameraRay);

    const auto moveDisplacement = tPositiveRayProjection > 0 ? tPositiveRayProjection * glm::normalize(handleDirection) : tNegativeRayProjection * glm::normalize(-handleDirection);
    return moveDisplacement;
}

std::optional<glm::vec3> GizmoSystem::getMouseIntersectionWithHandle() const
{
    if(!_selectedHandle)
    {
        return std::nullopt;
    }
    const auto& handleCollider = _registry->getComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(*_selectedHandle);
    const auto gizmoTransform = TransformComponent::getGlobalTransformMatrix(*_selectedHandle, _registry);
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());
    const auto& cameraTransformComponent = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto mousePosition = InputManager::getInstance().getRelativeMousePosition(InputManager::CoordinateSystem::SCENE);
    const auto cameraRay = cameraComponent.getRayAt(cameraTransformComponent, mousePosition.x, 1 - mousePosition.y);
    const auto intersectionDistanceOpt = handleCollider.box.intersect(cameraRay, gizmoTransform);
    return intersectionDistanceOpt ? std::make_optional(cameraRay.at(*intersectionDistanceOpt)) : std::nullopt;
}

void GizmoSystem::adjustGizmoTransform()
{
    _registry->getComponent<TransformComponent>(_translationGizmo).setScale({0, 0, 0});
    _registry->getComponent<TransformComponent>(_scaleGizmo).setScale({0, 0, 0});
    _registry->getComponent<TransformComponent>(_rotationGizmo).setScale({0, 0, 0});
    if(!_controlledEntity.has_value())
    {
        return;
    }

    Entity selected;
    switch(_mode)
    {
        case Mode::TRANSLATION:
            selected = _translationGizmo;
            break;
        case Mode::SCALE:
            selected = _scaleGizmo;
            break;
        default:
            return;
    }

    // Scale Gizmo
    _registry->getHierarchy().attributeChild(*_controlledEntity, _scaleGizmo);

    // Translation Gizmo
    auto& selectedEntityTransformComponent = _registry->getComponent<TransformComponent>(*_controlledEntity);
    const auto selectedEntityTargetTransformMatrix = TransformComponent::getGlobalTransformMatrix(*_controlledEntity, _registry);

    glm::vec3 correctionScale{1.0};
    if(selected == _scaleGizmo)
    {
        correctionScale.x = glm::length(glm::vec3(selectedEntityTargetTransformMatrix[0]));
        correctionScale.y = glm::length(glm::vec3(selectedEntityTargetTransformMatrix[1]));
        correctionScale.z = glm::length(glm::vec3(selectedEntityTargetTransformMatrix[2]));
    }

    const auto& cameraEntity = _registry->getCamera();
    const auto& cameraPosition = _registry->getComponent<TransformComponent>(cameraEntity).getPosition();
    const auto selectedEntityPosition = glm::vec3(selectedEntityTargetTransformMatrix[3]);
    const glm::vec3 gizmoToCamera = cameraPosition - selectedEntityPosition;
    const float gizmoToCameraDistance = std::sqrt((gizmoToCamera.x * gizmoToCamera.x) + (gizmoToCamera.y * gizmoToCamera.y) + (gizmoToCamera.z * gizmoToCamera.z));
    _registry->getComponent<TransformComponent>(_translationGizmo).setPosition(selectedEntityPosition);

    auto gizmoScale = glm::vec3{GizmoScale * gizmoToCameraDistance, GizmoScale * gizmoToCameraDistance, GizmoScale * gizmoToCameraDistance};
    _registry->getComponent<TransformComponent>(selected).setScale(gizmoScale / correctionScale);
}

}
