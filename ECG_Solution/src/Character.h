#pragma once

#include <PxPhysicsAPI.h>
#include "Camera.h"

using namespace physx;


class Character {

private:
	Camera* playerCamera = nullptr;
	
public:
	PxController* pxChar = nullptr;
	glm::vec3 charPosition;
	glm::vec3 direction;
	glm::vec3 up = glm::vec3(0, 1, 0);
	double mummyHeight;
	float  charSpeed = 2;
	bool superSpeed = false;

	Character();
	void createCharacter(Camera* camera, PxControllerManager* gManager, PxMaterial* material, glm::vec3 position);
	PxRigidDynamic* getActor();
	glm::vec3 getPosition();

	void move(glm::vec3 dir, float dt);
	void pollInput(GLFWwindow* window, float dt);
	void setPosition();
	void getBackToStart();
	void printPosition();

};