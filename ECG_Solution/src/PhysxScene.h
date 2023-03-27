#pragma once

#include "Utils.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"
#include <mesh.h>

#include "stb_image.h"
#include "PhysxCamera.h"

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
	
	PxMaterial* material = NULL;
	PxScene* scene = NULL;
	PxCooking* cooking = NULL;

	PxPvd* pvd = NULL;

	PxRigidDynamic* playerActor;
	PhysxCamera playerCamera;

public:
	PhysxScene();
	void simulate(GLFWwindow* window, float timeStep);
	void createTerrain(const char* heightmapPath);
	void createModel(std::vector<unsigned int> indices,std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 scale, glm::vec3 rotate);
	void createPlayer();
	PxScene* getScene();
};