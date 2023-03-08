#pragma once

#include "Light.h"

class DirectionalLight: public Light {
private:
    glm::vec3 direction;
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 intensity);
    glm::vec3 getDirection() const;
};