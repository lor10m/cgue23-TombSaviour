#pragma once

#define _USE_MATH_DEFINES
#include "Utils.h"
#include <cmath>

class Camera {
private:
    double pitch;
    double yaw;
    float distance;
    glm::vec3 targetPosition{};
    glm::mat4 perspectiveMatrix{};
    glm::vec3 right{};
    glm::vec3 up{};
    double pxpos = -1;
    double pypos = -1;
public:
    glm::vec3 pos;
    Camera(GLFWwindow* window, double fov, double aspect_ratio, double near, double far);
    void registerMovementCallbacks(GLFWwindow *window);
    static glm::mat4 getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t);
    glm::mat4 getOrbitCameraTransform();
    glm::mat4 getTransformMatrix();
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
