#pragma once

#include "Utils/Utils.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "Utils/stb_image.h"

#include "Drawables/Model.h"

using namespace physx;


class PhysxScene
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

public:
	PxMaterial* material = NULL;
	PxScene* scene = NULL;

	PhysxScene();
	void simulate(GLFWwindow* window, float timeStep);
	void createTerrain(const char* heightmapPath);
	void createModel(std::vector<unsigned int> indices, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotate);
	void createPlayer();
	PxScene* getScene();
};