#pragma once

#include "Utils.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"

#include "stb_image.h"
#include "PhysxCamera.h"

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
	void createPlayer(PhysxCamera playerCam);
};