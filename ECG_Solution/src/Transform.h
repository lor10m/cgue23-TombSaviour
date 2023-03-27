#pragma once

#define _USE_MATH_DEFINES
#include "Utils.h"
#include <cmath>

class Transform {
private:
    glm::mat4 transformMatrix = glm::transpose(glm::mat4({
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }));
    glm::mat4* viewTransform = nullptr;
public:
    glm::mat4 getMatrix() const;
    Transform& setViewTransform(glm::mat4* viewTransformMatrix);
    Transform& translate(glm::vec3 translationVec);
    Transform& rotate(glm::vec3 rotationVec);
    Transform& scale(glm::vec3 scalingVec);
};
