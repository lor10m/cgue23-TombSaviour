#include "Character.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>


Character::Character(Camera* playerCamera2, PxController* pxChar2)
{
	playerCamera = playerCamera2;
	pxChar = pxChar2;
	setPosition();
}

void Character::move(glm::vec3 dir, float dt)
{
	float moveX = dir.x * charSpeed;
	float moveZ = dir.z * charSpeed;

	pxChar->move(physx::PxVec3(moveX, -100.0f, moveZ) * dt, 0.001f, dt, physx::PxControllerFilters());
	setPosition();
}

void Character::pollInput(GLFWwindow* window, float dt) {

	direction = playerCamera->getDirection();		// cameraFront
	glm::vec3 dirForward{ 0.0f, 0.0f, 0.0f };
	glm::vec3 dirRight  { 0.0f, 0.0f, 0.0f };

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		getBackToStart();
		//std::cout << "B!";
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		dirForward += direction;
		//std::cout << "W!";
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		dirForward -= direction;
		//std::cout << "S!";
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		dirRight -= glm::normalize(glm::cross(direction, up));
		//std::cout << "A!";
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		dirRight += glm::normalize(glm::cross(direction, up));
		//std::cout << "D!";
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && superSpeed) {
		charSpeed = 4;
		superSpeed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		charSpeed = 8;
		superSpeed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
		return;	// it shouldn't do anything (also not just setting the same position again if no key is pressed
	}
	move(normalize(dirForward + dirRight), dt);
}

void Character::setPosition() {
	physx::PxExtendedVec3 pos = pxChar->getPosition();
	charPosition.x = pos.x;
	charPosition.y = pos.y;
	charPosition.z = pos.z;

	playerCamera->setCameraPosition(glm::vec3(charPosition.x, charPosition.y, charPosition.z));
}

glm::vec3 Character::getPosition() {
	return charPosition;
}

void Character::getBackToStart() {		// get back to start position with "B"
	physx::PxExtendedVec3 newPosition = { 0.0f, 200.0f, 0.0f };
	pxChar->setPosition(newPosition);
	setPosition();
}

void Character::printPosition() { // print current Position of controller object + camera
	std::cout << "\nCharPos: " << charPosition.x << "CameraPos: " << playerCamera->getCameraPosition().x << "\n";
	std::cout << "CharPos: " << charPosition.y << "CameraPos: " << playerCamera->getCameraPosition().y << "\n";
	std::cout << "CharPos: " << charPosition.z << "CameraPos: " << playerCamera->getCameraPosition().z << "\n";
}