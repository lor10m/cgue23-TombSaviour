#pragma once

#include "Light.h"

class PointLight: public Light {
private:
    glm::vec3 attenuation;
public:
    PointLight(glm::vec3 position, glm::vec3 intensity, glm::vec3 attenuation);
    glm::vec3 getAttenuation() const;
};
