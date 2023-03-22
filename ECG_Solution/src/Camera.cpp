#include "Camera.h"
#include "Callbacks.h"

Camera::Camera(){}

Camera::Camera(GLFWwindow* window, double fovStart, double aspect_ratioStart, double nearStart, double farStart) {
	pitch = 0;
	yaw = -90;
	fov = fovStart;
	aspect_ratio = aspect_ratioStart;
	near = nearStart;
	far = farStart;
	perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
	registerMovementCallbacks(window);
}

glm::mat4 Camera::getCameraTransform(glm::vec3 g, glm::vec3 e, glm::vec3 t) {
	{
		using namespace glm;
		vec3 w = -normalize(g);
		vec3 u = normalize(cross(t, w));
		vec3 v = cross(w, u);

		return inverse(mat4(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(e, 1)));
	}
}

void Camera::registerMovementCallbacks(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);
	glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
		static_cast<Camera*>(glfwGetWindowUserPointer(w))->scrollCallback(w, xoffset, yoffset);
		});
}

void Camera::updateProjectionMatrix(float aspect_ration_new) {
	aspect_ratio = aspect_ration_new;
	perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
}

void Camera::pollInput(GLFWwindow* window) {
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
		viewSpeedAdjusted = glm::min(viewSpeed * 15, viewSpeedAdjusted *= (1.0 + (dist - maxDist) / maxDist));    //adjust this?
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

// get back to start position with Scrolling
void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cameraPosition = { 0.0f, 0.0f, 10.0f };
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
	return perspectiveMatrix * getFirstPersonCameraTransform();
}