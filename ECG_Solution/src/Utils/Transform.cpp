#include "Transform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

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
    glm::quat quatX = glm::angleAxis(rotationVec.x, glm::vec3(1.0, 0.0, 0.0));
    glm::quat quatY = glm::angleAxis(rotationVec.y, glm::vec3(0.0, 1.0, 0.0));
    glm::quat quatZ = glm::angleAxis(rotationVec.z, glm::vec3(0.0, 0.0, 1.0));
    glm::quat combinedQuat = quatZ * quatY * quatX;

    glm::mat4 rotationMatrix = glm::mat4_cast(combinedQuat);
    transformMatrix = transformMatrix * rotationMatrix;
    
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
