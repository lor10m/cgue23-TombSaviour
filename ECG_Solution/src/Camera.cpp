#include "Camera.h"
#include "Utils/Callbacks.h"

Camera::Camera() {}

Camera::Camera(GLFWwindow* inputWindow, double fovStart, double aspect_ratioStart, double nearStart, double farStart, bool hdu) {
	pitch = 0;
	yaw = -90;
	fov = fovStart;
	aspect_ratio = aspect_ratioStart;
	near = nearStart;
	far = farStart;
	window = inputWindow;
	if (!hdu) {
		perspectiveMatrix = (glm::mat4)glm::perspective(fov, aspect_ratio, near, far);
	}
	else {
		calculateOrthogonalProjection();
	}

	registerMovementCallbacks(window);
}

void Camera::calculateOrthogonalProjection() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	for (int i = 0; i < 16; i++) {
		projectionMatrixHDU[i % 4][i / 4] = projectionMatrix[i];			// correct??
	}
}

glm::mat4 Camera::getProjectionMatrixHDU() {
	calculateOrthogonalProjection();
	return projectionMatrixHDU;
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

void Camera::pollMousePosition(GLFWwindow* window, bool isShooterCamera) 
{

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//TODO: shooterCam
	if (isShooterCamera) {

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		const int threshold = 50;
		if (xpos < threshold || xpos > width - threshold || ypos < threshold || ypos > height - threshold)
		{
			glfwSetCursorPos(window, width / 2, height / 2);
			lastX = width / 2;
			lastY = height / 2;
		}

		double xoffset = xpos - lastX;
		double yoffset = (ypos - lastY) * -1;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 1.0f;
		yaw += xoffset * sensitivity;
		pitch += yoffset * sensitivity;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
	else {
		// Calculate center of window
		double centerX = width / 2.0;
		double centerY = height / 2.0;

		// distance of cursor from center of window
		double dx = xpos - centerX;
		double dy = ypos - centerY;

		// maximum distance from center of window
		double maxDist = glm::min(centerX, centerY);

		// view speed based on distance from center of the window
		double dist = sqrt(dx * dx + dy * dy);
		double viewSpeedAdjusted = viewSpeed;
		if (dist > maxDist) {
			viewSpeedAdjusted = glm::min(viewSpeed * 15, viewSpeedAdjusted *= (1.0 + (dist - maxDist) / maxDist));    //adjust this?
		}

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

		// If cursor is near the edge of the window => adjust yaw + pitch
		double edgeDist = glm::min(200.0, width * 0.075);       // adjust the edge here
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
}

/*
camera center try => not well working
void Camera::pollMousePosition(GLFWwindow* window) { // Polling Mouse Cursour

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Check if cursor is outside window boundaries (stop moving/viewing movement)
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

	// Calculate center of window
	double centerX = width / 2.0;
	double centerY = height / 2.0;

	// distance of cursor from center of window
	double dx = xpos - centerX;
	double dy = ypos - centerY;

	// maximum distance from center of window
	double maxDist = glm::min(centerX, centerY);

	// view speed based on distance from center of the window
	double dist = sqrt(dx * dx + dy * dy);
	double viewSpeedAdjusted = viewSpeed;
	if (dist > maxDist) {
		viewSpeedAdjusted = glm::min(viewSpeed * 15, viewSpeedAdjusted *= (1.0 + (dist - maxDist) / maxDist));    //adjust this?
		//std::cout << viewSpeedAdjusted;
	}

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

	// If cursor is near the edge of the window => adjust yaw + pitch
	double edgeDist = glm::min(200.0, width * 0.075);       // adjust the edge here
	double pitchAdjustment = 0.0;
	double yawAdjustment = 0.0;

	// Check if cursor is close to the edge
	edgeDist = glm::min(200.0, width * 0.075); // adjust the edge distance here
	if (xpos < edgeDist || xpos > width - edgeDist || ypos < edgeDist || ypos > height - edgeDist) {
		// Set cursor position to the center of the window
		glfwSetCursorPos(window, centerX, centerY);

		// Calculate the change in cursor position from the center of the window
		double deltaX = (xpos - centerX) * viewSpeedAdjusted;
		double deltaY = (ypos - centerY) * viewSpeedAdjusted;

		pitch -= deltaY;
		pitch = glm::clamp(pitch, -89.999, 89.999);
		yaw += deltaX;

		if (yaw > 360.0) {
			yaw -= 360.0;
		}
		else if (yaw < 0.0) {
			yaw += 360.0;
		}

		radPitch = pitch * M_PI / 180.0;
		radYaw = yaw * M_PI / 180.0;

		cameraFront = normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw)));

		// Update lastX and lastY to the center of the window
		lastX = centerX;
		lastY = centerY;
	}
}
*/

// get back to start position with Scrolling
void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cameraPosition = { 0.0f, 200.0f, 0.0f };
}

// view Matrix
glm::mat4 Camera::getFirstPersonCameraTransform() {
	using namespace glm;
	double radPitch = pitch * M_PI / 180.0;
	double radYaw = yaw * M_PI / 180.0;

	// Calculate necessary angles and positions for the camera transform
	vec3 direction = { cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw) };
	right = normalize(cross(direction, vec3(0.0, 1.0, 0.0)));
	up = normalize(cross(right, direction));

	return getCameraTransform(direction, cameraPosition, up);
}

//viewProjectionMatrix
glm::mat4 Camera::getTransformMatrix() {
	return perspectiveMatrix * getFirstPersonCameraTransform();
}

// for player character: 
void Camera::setCameraPosition(glm::vec3 newPosition)
{
	cameraPosition = { newPosition.x, newPosition.y, newPosition.z };
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec2 Camera::getRotation()
{
	return glm::vec2(yaw, pitch);
}

glm::vec3 Camera::getRotationDirection() {
	double radPitch = pitch * M_PI / 180.0;
	double radYaw = yaw * M_PI / 180.0;
	glm::vec3 rotationDirection = { cos(radPitch) * cos(radYaw), sin(radPitch), cos(radPitch) * sin(radYaw) };
	return rotationDirection;
}

glm::vec3 Camera::getDirection()
{
	glm::vec3 dir = cameraFront;
	if (cameraFront.x > 1) {
		dir.x = 1;
	}
	if (cameraFront.x < -1) {
		dir.x = -1;
	}
	if (cameraFront.z > 1) {
		dir.z = 1;
	}
	if (cameraFront.z < -1) {
		dir.z = -1;
	}
	return dir;
}


