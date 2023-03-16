#include "Camera.h"
#include "Callbacks.h"

glm::mat4 Camera::getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t) {
    {
        using namespace glm;
        vec3 w = -normalize(g);
        vec3 u = normalize(cross(t, w));
        vec3 v = cross(w, u);

        return inverse(mat4(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(e, 1)));
    }
}

Camera::Camera(GLFWwindow* window, double fovStart, double aspect_ratioStart, double nearStart, double farStart) {
    pitch = 0;
    yaw = -90;
    //targetPosition = glm::vec3(0, 0, 0);
    //cameraPosition = glm::vec3(0, 0, 10);
    fov = fovStart;
    aspect_ratio = aspect_ratioStart;
    near = nearStart;
    far = farStart;
    perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
    registerMovementCallbacks(window);
}

void Camera::registerMovementCallbacks(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->scrollCallback(w, xoffset, yoffset);
        });
    /*
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->cursorPosCallback(w, xpos, ypos);
        });
     
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->keyCallback(w, key, scancode, action, mods);
        });
    */
}

void Camera::updateProjectionMatrix(float aspect_ration_new) {
    aspect_ratio = aspect_ration_new;
    perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
}

// WASD movement:
/*
void Camera::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPosition += cameraSpeed * cameraFront;
        std::cout << "W!";
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPosition -= cameraSpeed * cameraFront;
        std::cout << "S!";
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        //std::cout << "camera Front: " << cameraFront.x << cameraFront.y << cameraFront.z;
        //cameraPosition -= glm::vec3(1.0f, 0.0f, 0.0f);
        std::cout << "A!";
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        //cameraPosition.x += 1;
        std::cout << "D!";
    }
}
*/
void Camera::pollInput(GLFWwindow* window) {
    // if WASD should be solved with polling: (I wouldn't recommend this)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * cameraFront;
        //std::cout << "W!";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * cameraFront;
        //std::cout << "S!";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        //std::cout << "A!";
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        //std::cout << "D!";
    }
}

void Camera::pollMousePosition(GLFWwindow* window) { // Polling Mouse Cursour
    /* old one: 
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double deltaX = (xpos - lastX) * viewSpeed;
    double deltaY = (ypos - lastY) * viewSpeed;

    pitch -= deltaY;
    pitch = glm::min(glm::max(-89.999, pitch), 89.999);
    yaw += deltaX;
    //yaw = fmod(yaw, 360.0);

    // Calculate the camera's direction vector based on pitch and yaw angles
    double radPitch = pitch * M_PI / 180.0;
    double radYaw = yaw * M_PI / 180.0;

    cameraFront = normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)));

    lastX = xpos;
    lastY = ypos;
    */
    /*
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate the center of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double centerX = width / 2.0;
    double centerY = height / 2.0;

    // Calculate the distance of the cursor from the center of the window
    double dx = xpos - centerX;
    double dy = ypos - centerY;

    // Calculate the maximum distance from the center of the window
    double maxDist = glm::min(centerX, centerY);

    // Calculate the view speed based on the distance from the center of the window
    double dist = sqrt(dx * dx + dy * dy);
    double viewSpeedAdjusted = viewSpeed;
    if (dist > maxDist) {
        viewSpeedAdjusted *= 1.0 + (dist - maxDist) / maxDist;
    }

    // Calculate the delta values
    double deltaX = (xpos - lastX) * viewSpeedAdjusted;
    double deltaY = (ypos - lastY) * viewSpeedAdjusted;

    pitch -= deltaY;
    pitch = glm::clamp(pitch, -89.999, 89.999);
    yaw += deltaX;

    if (yaw > 360.0) {
        yaw -= 360.0;
    }
    else if (yaw < 0.0) {
        yaw += 360.0;
    }

    double radPitch = pitch * M_PI / 180.0;
    double radYaw = yaw * M_PI / 180.0;

    cameraFront = normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)));

    lastX = xpos;
    lastY = ypos;

    // If the cursor is outside the window, set it to the center of the window
    if (xpos < 0 || xpos > width || ypos < 0 || ypos > height) {
        glfwSetCursorPos(window, centerX, centerY);
        lastX = centerX;
        lastY = centerY;
        firstMouse = true;
    }
    */
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Check if the cursor is outside the window boundaries
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (xpos < 0 || xpos >= width || ypos < 0 || ypos >= height) {
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate the center of the window
    double centerX = width / 2.0;
    double centerY = height / 2.0;

    // Calculate the distance of the cursor from the center of the window
    double dx = xpos - centerX;
    double dy = ypos - centerY;

    // Calculate the maximum distance from the center of the window
    double maxDist = glm::min(centerX, centerY);

    // Calculate the view speed based on the distance from the center of the window
    double dist = sqrt(dx * dx + dy * dy);
    double viewSpeedAdjusted = viewSpeed;
    if (dist > maxDist) {
        viewSpeedAdjusted = glm::min(viewSpeed*15, viewSpeedAdjusted *= (1.0 + (dist - maxDist) / maxDist));    //adjust this?
        //std::cout << viewSpeedAdjusted;
    }

    // Calculate the delta values
    double deltaX = (xpos - lastX) * viewSpeedAdjusted;
    double deltaY = (ypos - lastY) * viewSpeedAdjusted;

    pitch -= deltaY;
    pitch = glm::clamp(pitch, -89.999, 89.999);
    yaw += deltaX;

    if (yaw > 360.0) {
        yaw -= 360.0;
    }
    else if (yaw < 0.0) {
        yaw += 360.0;
    }

    double radPitch = pitch * M_PI / 180.0;
    double radYaw = yaw * M_PI / 180.0;

    cameraFront = normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)));

    lastX = xpos;
    lastY = ypos;

    // If the cursor is near the edge of the window, adjust the yaw and pitch values
    double edgeDist = glm::min(200.0, width * 0.075);       // adjust here the edge
    double pitchAdjustment = 0.0;
    double yawAdjustment = 0.0;

    if (xpos < edgeDist) {
        yawAdjustment = -((edgeDist - xpos) / edgeDist);
    }
    else if (xpos > width - edgeDist) {
        yawAdjustment = (xpos - (width - edgeDist)) / edgeDist;
    }
    if (ypos < edgeDist) {
        pitchAdjustment = (edgeDist - ypos) / edgeDist;
    }
    else if (ypos > height - edgeDist) {
        pitchAdjustment = -((ypos - (height - edgeDist)) / edgeDist);
    }

    pitch += pitchAdjustment * viewSpeedAdjusted;
    pitch = glm::clamp(pitch, -89.999, 89.999);
    yaw += yawAdjustment * viewSpeedAdjusted;
    yaw = glm::mod(yaw, 360.0);
}

/*
void Camera::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (pxpos >= 0) {
            double deltaX = xpos - pxpos;
            double deltaY = ypos - pypos;
            pitch -= deltaY;
            pitch = glm::min(glm::max(-89.999, pitch), 89.999);
            yaw += deltaX;
            yaw = fmod(yaw, 360.0);

            // Calculate the camera's direction vector based on pitch and yaw angles
            double radPitch = pitch * M_PI / 180.0;
            double radYaw = yaw * M_PI / 180.0;

            cameraFront = normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)
            ));
        }
        pxpos = xpos;
        pypos = ypos;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (pxpos >= 0) {
            double deltaX = xpos - pxpos;
            double deltaY = ypos - pypos;
            targetPosition -= (float)deltaX / 50 * right;
            targetPosition += (float)deltaY / 50 * up;
            cameraFront = targetPosition - cameraPosition;
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
*/

// get back to start position with Scrolling
void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    /*distance -= (float)yoffset;
    distance = glm::max(1.0f, distance);*/
    //std::cout << "camera Position: " << cameraPosition.x << cameraPosition.y << cameraPosition.z;
    //std::cout << "camera Front: " << cameraFront.x << cameraFront.y << cameraFront.z;
    cameraPosition = { 0.0f, 0.0f, 10.0f };
    //std::cout << "camera Position: " << cameraPosition.x << cameraPosition.y << cameraPosition.z;
}

glm::mat4 Camera::getOrbitCameraTransform() {
    {
        using namespace glm;
        double radPitch = pitch * M_PI / 180.0;
        double radYaw = yaw * M_PI / 180.0;

        // Calculate necessary angles and positions for the camera transform
        vec3 direction = { cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw) };
        right = normalize(cross(direction, vec3(0.0, 1.0, 0.0)));
        up = normalize(cross(right, direction));

        //cameraPosition = (-direction * distance) + targetPosition;
        cameraPosition = cameraPosition;

        return Camera::getCameraTransform(direction, cameraPosition, up);
    }
}

glm::mat4 Camera::getFirstPersonCameraTransform() {
    using namespace glm;
    double radPitch = pitch * M_PI / 180.0;
    double radYaw = yaw * M_PI / 180.0;

    // Calculate necessary angles and positions for the camera transform
    vec3 direction = { cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw) };
    right = normalize(cross(direction, vec3(0.0, 1.0, 0.0)));
    up = normalize(cross(right, direction));

    return Camera::getCameraTransform(direction, cameraPosition, up);
}

glm::mat4 Camera::getTransformMatrix() {
    //return perspectiveMatrix * getOrbitCameraTransform();
    return perspectiveMatrix * getFirstPersonCameraTransform();
}