#pragma once

#include "Utils/Utils.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "Utils/stb_image.h"
#include "Utils/GlobalVariables.h"
#include "PxSimulationEventCallback.h"

#include "Drawables/Model.h"
#include "Character.h"
#include "Hdu.h"

using namespace physx;

struct DynamicActor {
	unsigned int index;
	PxRigidDynamic* actor;
	bool isThrownOrPickedUp = false;
};

struct StaticActor {
	unsigned int index;
	PxRigidStatic* actor;
	bool isThrownOrPickedUp = false;
};

class PhysxScene : public PxSimulationEventCallback
{
private:

	PxDefaultAllocator defaultAllocatorCallback;
	PxDefaultErrorCallback defaultErrorCallback;
	PxDefaultCpuDispatcher* dispatcher = NULL;
	PxTolerancesScale toleranceScale;
	PxFoundation* foundation = NULL;
	PxPhysics* physics = NULL;
	PxCooking* cooking = NULL;
	PxPvd* pvd = NULL;
	PxRigidDynamic* playerActor;

	unsigned int mummyLiveCount = 5;
	unsigned int maxLives = 3;

	Character* mummy;
	Hdu* hdu;

	PxRigidDynamic* cactus;
	PxRigidDynamic* spike;
	PxFixedJoint* joint;

	bool pickedUp;

	bool mouse_right_pressed = false;
	bool mouse_right_released = true;
	bool mouse_left_pressed = false;
	bool mouse_left_released = true;

	Shader* cactusShader;
	glm::mat4 cactusModelMatrix;

	std::map<unsigned int, DynamicActor> spikes;
	std::map<unsigned int, DynamicActor> cacti;

	std::vector<PxActor*> actorsToRemove;


public:
	PxMaterial* material = NULL;
	PxScene* scene = NULL;
	bool allEnemiesDead = false;

	unsigned int pickedUpSpikes = 0;
	unsigned int thrownSpikes = 0;
	unsigned int spickesPerCactus = 5;
	//unsigned int lifeCnt = 3;
	//int maxLifeNr = 3;

	std::vector<unsigned int> enemiesToRemove;

	PhysxScene(GLFWwindow* window, int lifeNumber);
	void simulate(GLFWwindow* window, Camera* camera, float timeStep, std::map<unsigned int, SpikeStruct>& spikeStruct, std::map<unsigned int, CactusStruct>& cactusStruct);
	void createTerrain(const char* heightmapPath);
	void createModel(const char* name, std::vector<unsigned int> indices, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotate);

	void setCharacter(Character* character);
	void setHDU(Hdu* hduObj);
	void createCactus(unsigned int index, glm::vec3 size, glm::vec3 position);
	void createSpike(unsigned int index, glm::vec3 size, glm::vec3 position);

	void throwSpike(Camera* camera);
	void pickUpObject(Camera* camera, PxRigidDynamic* object);
	void pickUpNearestObject(Camera* camera);
	PxScene* getScene();

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	bool enemiesRemaining();
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor**, PxU32) {}
	virtual void onSleep(PxActor**, PxU32) {}
	virtual void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}

	void mouseButtonCallback(GLFWwindow* window, Camera* camera);

	unsigned int getMummyLiveCount();

	void deleteScene();

	void decreaseMummyLive();
};