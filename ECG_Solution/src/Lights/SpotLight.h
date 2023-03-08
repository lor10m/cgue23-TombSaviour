#pragma once

#include "Light.h"

class SpotLight: public Light {
private:
    glm::vec3 attenuation;
    glm::vec3 direction;
    float angle;
public:
    SpotLight(glm::vec3 position, glm::vec3 intensity, glm::vec3 direction, float angle, glm::vec3 attenuation);
    glm::vec3 getAttenuation() const;
    glm::vec3 getDirection() const;
    float getAngle() const;
};