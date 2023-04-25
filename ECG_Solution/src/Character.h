#pragma once

#include <PxPhysicsAPI.h>
#include "Camera.h"

using namespace physx;


class Character {

private:
	Camera* playerCamera;
	
public:
	PxController* pxChar;
	glm::vec3 charPosition;
	glm::vec3 direction;
	glm::vec3 up = glm::vec3(0, 1, 0);
	double mummyHeight;
	float  charSpeed = 10;
	bool superSpeed = false;

	Character();
	void createCharacter(GLFWwindow* window, Camera* camera, PxControllerManager* gManager, PxMaterial* material, glm::vec3 position);
	PxRigidDynamic* getActor();
	glm::vec3 getPosition();

	void move(glm::vec3 dir, float dt);
	void pollInput(GLFWwindow* window, float dt);
	void setPosition();
	void getBackToStart();
	void printPosition();

	glm::vec3 dirForward{ 0.0f, 0.0f, 0.0f };
	glm::vec3 dirRight{ 0.0f, 0.0f, 0.0f };

	float moveX = 0.0f;
	float moveZ = 0.0f;
	physx::PxVec3 disp;

	bool a_pressed = false;
	bool d_pressed = false;
	bool setnewAtime = true;
	bool setnewDtime = true;
	double time_a_pressed = 0.0;
	double time_d_pressed = 0.0;

	bool w_pressed = false;
	bool s_pressed = false;
	bool setnewWtime = true;
	bool setnewStime = true;
	double time_w_pressed = 0.0;
	double time_s_pressed = 0.0;

	glm::vec3 getWSdirection(GLFWwindow* window);
	glm::vec3 getADdirection(GLFWwindow* window);

};