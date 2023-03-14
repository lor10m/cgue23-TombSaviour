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
    glm::vec3 up { 0.0f, 1.0f, 0.0f };
    double pxpos = -1;
    double pypos = -1;

    float cameraSpeed = 1.0;

public:
    glm::vec3 cameraPosition{ 0.0f, 0.0f, 10.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, 1.0f};

    Camera(GLFWwindow* window, double fov, double aspect_ratio, double near, double far);
    void registerMovementCallbacks(GLFWwindow *window);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void updateVectors();
    static glm::mat4 getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t);
    glm::mat4 getOrbitCameraTransform();
    glm::mat4 getFirstPersonCameraTransform();
    glm::mat4 getTransformMatrix();
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void moveWASDCallback(GLFWwindow* window);
};
