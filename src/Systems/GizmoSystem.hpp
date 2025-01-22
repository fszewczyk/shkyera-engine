#pragma once

#include <glm/glm.hpp>

#include <ECS/Registry.hpp>

namespace shkyera {

class GizmoSystem {
    public:
        GizmoSystem(std::shared_ptr<Registry> registry);
        ~GizmoSystem();

        void update();

    private:
        inline static const auto Red = glm::vec3{0.9, 0.1, 0.1};
        inline static const auto Green = glm::vec3{0.1, 0.9, 0.3};
        inline static const auto Blue = glm::vec3{0.1, 0.45, 0.95};
        inline static const auto Grey = glm::vec3{0.8, 0.8, 0.8};

        static constexpr float GizmoLineWidth = 0.02;
        static constexpr float GizmoLineLength = 1.0;
        static constexpr float GizmoHandleSize = 0.1;
        static constexpr float GizmoScale = 0.07;

        enum class Mode {
            TRANSLATION,
            SCALE,
        };

        void moveEntity();
        void scaleEntity();

        void selectEntity();
        void styleOnHover();
        void adjustGizmoTransform();

        bool isHoverMoved(std::optional<Entity> currentHover) const;
        glm::vec3 getSelectedHandleDisplacement() const;
        std::optional<glm::vec3> getMouseIntersectionWithHandle() const;

        // Global Info
        Mode _mode = Mode::TRANSLATION;
        std::shared_ptr<Registry> _registry;
        glm::vec3 _originalColor;
        std::optional<Entity> _selectedHandle;
        std::optional<Entity> _hoveredHandle;
        std::optional<Entity> _controlledEntity;

        // Translation Info
        Entity _translationGizmo;
        glm::vec3 _originalHandlePosition;
        glm::vec3 _originalDisplacement;

        // Scale Info
        Entity _scaleGizmo;
        Entity _scaleUniformHandle, _scaleGizmoLineX, _scaleGizmoLineY, _scaleGizmoLineZ;
        glm::vec3 _originalScale;

        // Rotatin Info
        Entity _rotationGizmo;
        glm::vec3 _originalPlaneIntersection;
        glm::vec3 _originalOrientation;
};

}