
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
	float  charSpeed = 4;
	bool superSpeed = false;

	Character(Camera* camera, PxController* pxChar);
	glm::vec3 getPosition();

	void move(glm::vec3 dir, float dt);
	void pollInput(GLFWwindow* window, float dt);
	void setPosition();
	void getBackToStart();
	void printPosition();

};