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
    double lastX, lastY;
    bool firstMouse = true;

    double fov, aspect_ratio, near, far = 0.0;

public:
    float cameraSpeed = 1.0;
    double viewSpeed = 0.1;
    glm::vec3 cameraPosition{ 0.0f, 0.0f, 10.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f};      //if not negative WASD would start in opposite directions

    Camera(GLFWwindow* window, double fov, double aspect_ratio, double near, double far);
    void registerMovementCallbacks(GLFWwindow *window);
    static glm::mat4 getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t);
    glm::mat4 getOrbitCameraTransform();
    glm::mat4 getFirstPersonCameraTransform();
    glm::mat4 getTransformMatrix();
    void pollInput(GLFWwindow* window);
    void pollMousePosition(GLFWwindow* window);
    //void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void setCameraPosition(string WASD);

    void updateProjectionMatrix(float aspect_ration_new);

    //void moveWASDCallback(GLFWwindow* window);
};
