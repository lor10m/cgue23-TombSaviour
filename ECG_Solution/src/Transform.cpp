#include "Transform.h"

glm::mat4 Transform::getMatrix() const {
    return transformMatrix;
    if (viewTransform == nullptr) {
        return transformMatrix;
    }
    return *viewTransform * transformMatrix;
}

Transform& Transform::translate(glm::vec3 translationVec) {
    transformMatrix = glm::translate(transformMatrix, translationVec);
    return *this;
}

Transform& Transform::rotate(glm::vec3 rotationVec) {
    transformMatrix = glm::rotate(transformMatrix, rotationVec.z, glm::vec3(0.0, 0.0, 1.0)) * transformMatrix;
    transformMatrix = glm::rotate(transformMatrix, rotationVec.y, glm::vec3(0.0, 1.0, 0.0)) * transformMatrix;
    transformMatrix = glm::rotate(transformMatrix, rotationVec.x, glm::vec3(1.0, 0.0, 0.0)) * transformMatrix;
    return *this;
}

Transform& Transform::setViewTransform(glm::mat4* viewTransformMatrix) {
    viewTransform = viewTransformMatrix;
    return *this;
}

Transform& Transform::scale(glm::vec3 scalingVec) {
    transformMatrix = glm::scale(transformMatrix, scalingVec);
    return *this;
}
