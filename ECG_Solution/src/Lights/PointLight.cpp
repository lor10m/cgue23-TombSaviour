#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 intensity, glm::vec3 attenuation) {
    this->position = position;
    this->intensity = intensity;
    this->attenuation = attenuation;
}

glm::vec3 PointLight::getAttenuation() const {
    return attenuation;
}
