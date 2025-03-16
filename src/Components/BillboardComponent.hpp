#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

template <RuntimeMode Mode = RuntimeMode::PRODUCTION>
struct BillboardComponent {
   public:
    enum class Orientation {
        CameraFacing,
        AxisAligned,
        WorldFixed
    };

    enum class Scale {
        World,
        Camera
    };

    enum class Occlusion {
        Occludable,
        NotOccludable
    };

    Orientation orientation{Orientation::CameraFacing};
    Scale scale{Scale::Camera};
    Occlusion occlusion{Occlusion::NotOccludable};
    HandleAndAsset<Material> material{};

    glm::mat4 getModelMatrix(const glm::mat4& baseMatrix, const glm::vec3& cameraPosition, const glm::mat4& viewMatrix) const {
        glm::vec3 position = glm::vec3(baseMatrix[3]);

        glm::vec3 scaleVec(
            glm::length(glm::vec3(baseMatrix[0])),
            glm::length(glm::vec3(baseMatrix[1])),
            glm::length(glm::vec3(baseMatrix[2])));

        if (scale == Scale::Camera) {
            float distance = glm::distance(cameraPosition, position);
            scaleVec *= distance / 30.0f;
        }

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

        glm::mat4 rotation(1.0f);
        if (orientation == Orientation::CameraFacing) {
            glm::mat3 camRot = glm::mat3(viewMatrix);
            camRot = glm::transpose(camRot);
            rotation = glm::mat4(camRot);
            rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0, 0, 1));
        } else if (orientation == Orientation::AxisAligned) {
            glm::vec3 dir = cameraPosition - position;
            dir.y = 0.0f;  // ignore vertical component
            if (glm::length(dir) > 0.0001f) {
                dir = glm::normalize(dir);
            }
            float angle = std::atan2(dir.x, dir.z);
            rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        } else if (orientation == Orientation::WorldFixed) {
            glm::vec3 right = glm::normalize(glm::vec3(baseMatrix[0]));
            glm::vec3 up = glm::normalize(glm::vec3(baseMatrix[1]));
            glm::vec3 forward = glm::normalize(glm::vec3(baseMatrix[2]));
            glm::mat3 R_base;
            R_base[0] = right;
            R_base[1] = up;
            R_base[2] = forward;
            rotation = glm::mat4(R_base);
        }

        glm::mat4 correction = glm::rotate(glm::mat4(1.0f),
                                           glm::radians(90.0f),
                                           glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scaleVec);

        glm::mat4 model = translation * rotation * correction * scaling;
        return model;
    }
};

}  // namespace shkyera
