#pragma once

#include "../Utils.h"

class Light {
protected:
    glm::vec3 intensity{};
    glm::vec3 position{};
public:
    glm::vec3 getPosition() const {
        return position;
    }
    glm::vec3 getIntensity() const {
        return intensity;
    }
};
