#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 intensity) {
    this->direction = direction;
    this->intensity = intensity;
}

glm::vec3 DirectionalLight::getDirection() const {
    return direction;
}
