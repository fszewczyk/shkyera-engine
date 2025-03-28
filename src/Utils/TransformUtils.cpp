#include <glm/gtc/matrix_transform.hpp>
#include <Utils/TransformUtils.hpp>

namespace shkyera::utils::transform {

glm::mat4 getCameraFacingModelMatrix(const glm::mat4& baseMatrix, const glm::vec3& cameraPosition, const glm::mat4& viewMatrix)
{
    glm::vec3 position = glm::vec3(baseMatrix[3]);

    glm::vec3 scaleVec(
        glm::length(glm::vec3(baseMatrix[0])),
        glm::length(glm::vec3(baseMatrix[1])),
        glm::length(glm::vec3(baseMatrix[2]))
    );

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

    glm::mat4 rotation(1.0f);
    glm::mat3 camRot = glm::mat3(viewMatrix);
    camRot = glm::transpose(camRot);
    rotation = glm::mat4(camRot);
    rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0, 0, 1));

    glm::mat4 correction = glm::rotate(glm::mat4(1.0f),
                                        glm::radians(90.0f),
                                        glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scaleVec);

    glm::mat4 model = translation * rotation * correction * scaling;
    return model;
}

}