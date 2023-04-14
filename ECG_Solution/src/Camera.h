#pragma once

#define _USE_MATH_DEFINES
#include "Utils.h"
#include <cmath>

class Camera {
private:
    double pitch;
    double yaw;
    glm::vec3 targetPosition{};
    glm::mat4 perspectiveMatrix{};
    glm::vec3 up { 0.0f, 1.0f, 0.0f };
    double pxpos = -1;
    double pypos = -1;
    double lastX, lastY;
    bool firstMouse = true;

    double fov, aspect_ratio, near, far = 0.0;

public:
    double viewSpeed = 0.5;
    glm::vec3 cameraPosition{ 0.0f, 30.0f, 0.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f};      //if not negative WASD would start in opposite directions
    glm::vec3 right{};

    Camera::Camera();
    Camera(GLFWwindow* window, double fov, double aspect_ratio, double near, double far);
    
    static glm::mat4 getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t);
    glm::mat4 getFirstPersonCameraTransform();
    glm::mat4 getTransformMatrix();
    void updateProjectionMatrix(float aspect_ration_new);

    void registerMovementCallbacks(GLFWwindow* window);
    void pollMousePosition(GLFWwindow* window);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void setCameraPosition(glm::vec3 newPosition);

    glm::vec2 Camera::getRotation();
    glm::vec3 Camera::getDirection();
    glm::vec3 Camera::getCameraPosition();
};
