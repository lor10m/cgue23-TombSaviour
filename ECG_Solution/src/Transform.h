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
    Transform& translate(float x, float y, float z);
    Transform& scale(float x, float y, float z);
    Transform& rotateY(float degrees);
};
