#include "Transform.h"

glm::mat4 Transform::getMatrix() const {
    return transformMatrix;
    if (viewTransform == nullptr) {
        return transformMatrix;
    }
    return *viewTransform * transformMatrix;
}

Transform& Transform::translate(float x, float y, float z) {
    transformMatrix = glm::transpose(glm::mat4({
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f,
    })) * transformMatrix;
    return *this;
}

Transform& Transform::rotateY(float degrees) {
    double rad = degrees * M_PI / 180.0f;
    transformMatrix = glm::transpose(glm::mat4({
        cos(rad), 0.0f, sin(rad), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(rad), 0.0f, cos(rad), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    })) * transformMatrix;
    return *this;
}

Transform& Transform::setViewTransform(glm::mat4* viewTransformMatrix) {
    viewTransform = viewTransformMatrix;
    return *this;
}

Transform& Transform::scale(float x, float y, float z) {
    transformMatrix = glm::transpose(glm::mat4({
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    })) * transformMatrix;
    return *this;
}
