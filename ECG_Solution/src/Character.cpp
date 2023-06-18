#include "Character.h"


Character::Character() {};

void Character::createCharacter(GLFWwindow* window, Camera* camera, PxControllerManager* gManager, PxMaterial* material, glm::vec3 position)
{
	PxCapsuleControllerDesc cDesc;
	cDesc.position = PxExtendedVec3{ position.x, position.y, position.z };
	cDesc.contactOffset = 0.05f;
	cDesc.height = 3.0f;
	cDesc.radius = 1.0f;
	cDesc.stepOffset = 0.2f;
	cDesc.slopeLimit = 0.2f;
	cDesc.upDirection = PxVec3(0.0f, 1.0f, 0.0f);
	cDesc.material = material;
	pxChar = gManager->createController(cDesc);
	pxChar->getActor()->setName("mummy");

	playerCamera = camera;
	setPosition();
}

void Character::move(glm::vec3 dir, float dt)
{
	moveX = dir.x * charSpeed;
	moveZ = dir.z * charSpeed;
	disp = physx::PxVec3(moveX, -100.0f, moveZ) * dt;

	pxChar->move(disp, 0.001f, dt, physx::PxControllerFilters());
	setPosition();
}

void Character::pollInput(GLFWwindow* window, float dt) {

	direction = playerCamera->getDirection();		// cameraFront

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		getBackToStart();
		//std::cout << "B!";
	}

	dirForward = getWSdirection(window);
	dirRight = getADdirection(window);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && superSpeed) {
		charSpeed = 10;
		superSpeed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		charSpeed = 20;
		superSpeed = true;
		printPosition();
		//std::cout << "Aspect ratio: " << playerCamera->aspect_ratio << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
		return;	// it shouldn't do anything (also not just setting the same position again if no key is pressed
	}
	move(normalize(dirForward + dirRight), dt);
}

void Character::setPosition() {
	physx::PxExtendedVec3 pos = pxChar->getPosition(); //getFootPosition()
	charPosition.x = pos.x;
	charPosition.y = pos.y;
	charPosition.z = pos.z;

	playerCamera->setCameraPosition(glm::vec3(charPosition.x, charPosition.y, charPosition.z));

	//printPosition();
}
PxRigidDynamic* Character::getActor() {
	return pxChar->getActor();
}

glm::vec3 Character::getPosition() {
	return charPosition;
}

void Character::getBackToStart() {		// get back to start position with "B"
	physx::PxExtendedVec3 newPosition = { 0.0f, 30.0f, 0.0f };
	pxChar->setPosition(newPosition);
	setPosition();
}

void Character::printPosition() { // print current Position of controller object + camera
	//std::cout << "\nCharPos: " << charPosition.x << "CameraPos: " << playerCamera->getCameraPosition().x << "\n";
	//std::cout << "CharPos: " << charPosition.y << "CameraPos: " << playerCamera->getCameraPosition().y << "\n";
	//std::cout << "CharPos: " << charPosition.z << "CameraPos: " << playerCamera->getCameraPosition().z << "\n";
	
	// DEBUG just for positioning: 
	physx::PxExtendedVec3 pos = pxChar->getFootPosition(); //getFootPosition()
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	std::cout << "\nCharPos Foot: " << x << "\n";
	std::cout << "CharPos Foot: " << y << "\n";
	std::cout << "CharPos Foot: " << z << "\n";
}

glm::vec3 Character::getWSdirection(GLFWwindow* window) {
	dirForward = glm::vec3(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		w_pressed = true;
		if (setnewWtime) {
			time_w_pressed = glfwGetTime();
			setnewWtime = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		s_pressed = true;
		if (setnewStime) {
			time_s_pressed = glfwGetTime();
			setnewStime = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		w_pressed = false;
		setnewWtime = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		s_pressed = false;
		setnewStime = true;
	}
	if (w_pressed && s_pressed) {
		// check which key was last pressed
		if (time_w_pressed > time_s_pressed) {
			dirForward += direction;
		}
		else {
			dirForward -= direction;
		}
	}
	else if (w_pressed) {
		dirForward += direction;
	}
	else if (s_pressed) {
		dirForward -= direction;
	}
	return dirForward;
}

glm::vec3 Character::getADdirection(GLFWwindow* window) {
	dirRight = glm::vec3(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		a_pressed = true;
		if (setnewAtime) {
			time_a_pressed = glfwGetTime();
			setnewAtime = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		d_pressed = true;
		if (setnewDtime) {
			time_d_pressed = glfwGetTime();
			setnewDtime = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
		a_pressed = false;
		setnewAtime = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		d_pressed = false;
		setnewDtime = true;
	}
	if (a_pressed && d_pressed) {
		// check which key was last pressed
		if (time_a_pressed > time_d_pressed) {
			dirRight -= glm::normalize(glm::cross(direction, up));
		}
		else {
			dirRight += glm::normalize(glm::cross(direction, up));
		}
	}
	else if (a_pressed) {
		dirRight -= glm::normalize(glm::cross(direction, up));
	}
	else if (d_pressed) {
		dirRight += glm::normalize(glm::cross(direction, up));
	}
	return dirRight;
}

