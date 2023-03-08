#include "Camera.h"

glm::mat4 Camera::getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t) {
    {
        using namespace glm;
        vec3 w = - normalize(g);
        vec3 u = normalize(cross(t, w));
        vec3 v = cross(w, u);

        return inverse(mat4(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(e, 1)));
    }
}

Camera::Camera(GLFWwindow* window, double fov, double aspect_ratio, double near, double far) {
    pitch = 0;
    yaw = -90;
    distance = 6;
    targetPosition = glm::vec3(0, 0, 0);
    perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
    registerMovementCallbacks(window);
}

void Camera::registerMovementCallbacks(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->scrollCallback(w, xoffset, yoffset);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->cursorPosCallback(w, xpos, ypos);
    });
}

void Camera::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (pxpos >= 0) {
            double deltaX = xpos - pxpos;
            double deltaY = ypos - pypos;
            pitch -= deltaY;
            pitch = glm::min(glm::max(-89.999, pitch), 89.999);
            yaw += deltaX;
            yaw = fmod(yaw, 360.0);
        }
        pxpos = xpos;
        pypos = ypos;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (pxpos >= 0) {
            double deltaX = xpos - pxpos;
            double deltaY = ypos - pypos;
            targetPosition -= (float)deltaX / 50 * right;
            targetPosition += (float)deltaY / 50 * up;
        }
        pxpos = xpos;
        pypos = ypos;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE
        && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        pxpos = -1;
        pypos = -1;
    }
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    distance -= (float)yoffset;
    distance = glm::max(1.0f, distance);
}

glm::mat4 Camera::getOrbitCameraTransform() {
    {
        using namespace glm;
        double radPitch = pitch * M_PI / 180.0;
        double radYaw = yaw * M_PI / 180.0;

        // Calculate necessary angles and positions for the camera transform
        vec3 direction = {cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)};
        right = normalize(cross(direction, vec3(0.0, 1.0, 0.0)));
        up = normalize(cross(right, direction));

        pos = (-direction * distance) + targetPosition;

        return Camera::getCameraTransform(direction, pos, up);
    }
}

glm::mat4 Camera::getTransformMatrix() {
    return perspectiveMatrix * getOrbitCameraTransform();
}