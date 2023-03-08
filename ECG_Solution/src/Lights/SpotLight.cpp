#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 intensity, glm::vec3 direction, float angle, glm::vec3 attenuation) {
    this->position = position;
    this->intensity = intensity;
    this->direction = direction;
    this->angle = angle;
    this->attenuation = attenuation;
}

glm::vec3 SpotLight::getAttenuation() const {
    return attenuation;
}

glm::vec3 SpotLight::getDirection() const {
    return direction;
}

float SpotLight::getAngle() const {
    return angle;
}
