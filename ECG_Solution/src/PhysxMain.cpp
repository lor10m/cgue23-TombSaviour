//#include "PxPhysicsAPI.h"
//#include "cooking/PxCooking.h"
//#include "cooking/PxTriangleMeshDesc.h"
//#include <iostream>
//#include "Utils.h"
//#include <vector>
//#include "Terrain.h"
//
//using namespace physx;
//void createStack();
//void createTerrain(PxScene* scene, PxPhysics* physics);
//
//// declare variables
//physx::PxDefaultAllocator      mDefaultAllocatorCallback;
//physx::PxDefaultErrorCallback  mDefaultErrorCallback;
//physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
//physx::PxTolerancesScale       mToleranceScale;
//
//physx::PxFoundation* mFoundation = NULL;
//physx::PxPhysics* mPhysics = NULL;
//
//physx::PxScene* mScene = NULL;
//physx::PxMaterial* mMaterial = NULL;
//PxCooking* mCooking = NULL;
//
//physx::PxPvd* mPvd = NULL;
//
//PxVec3 vertices[20*20*4];
//int indices[20*20*6];
//
//int main() {
//
//	// init physx
//	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
//	if (!mFoundation) throw("PxCreateFoundation failed!");
//
//	mPvd = PxCreatePvd(*mFoundation);
//	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
//	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
//	mToleranceScale.length = 100;        // typical length of an object
//	mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
//	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
//
//	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
//	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
//	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.cpuDispatcher = mDispatcher;
//	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
//	mScene = mPhysics->createScene(sceneDesc);
//
//	physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
//	if (pvdClient)
//	{
//		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//	}
//
//	// create simulation
//	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//	physx::PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 50), *mMaterial);
//	mScene->addActor(*groundPlane);
//
//	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(PxTolerancesScale()));
//	if (!mCooking) {
//		EXIT_WITH_ERROR("Failed to init cooking")
//	}
//
//	//createStack(mMaterial, mPhysics, mScene);
//	createTerrain(mScene, mPhysics);
//	createStack();
//	// run simulation
//	while (1) {
//		mScene->simulate(1.0f / 60.0f);
//		mScene->fetchResults(true);
//	}
//}
//
//void createTerrain(PxScene* scene, PxPhysics* physics) {
//
//	int width, height, channels;
//	// Load the heightmap image
//	unsigned char* image = stbi_load("assets/heightmaps/hm3.png", &width, &height, &channels, STBI_grey);
//
//	// Create the height field samples buffer
//	PxHeightFieldSample* samples = new PxHeightFieldSample[width * height];
//
//	// Set the height field sample data from the image data
//	for (int y = 0; y < height; y++) {
//		for (int x = 0; x < width; x++) {
//			samples[y * width + x].height = static_cast<PxI16>(image[y * width + x]);
//		}
//	}
//
//	// Create the height field descriptor
//	PxHeightFieldDesc heightFieldDesc;
//	heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
//	heightFieldDesc.nbColumns = width;
//	heightFieldDesc.nbRows = height;
//	heightFieldDesc.samples.data = samples;
//	heightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);
//
//	// Create the height field
//	PxHeightField* heightField = mCooking->createHeightField(heightFieldDesc,physics->getPhysicsInsertionCallback());
//
//	// Create the height field geometry
//	PxHeightFieldGeometry heightFieldGeom(heightField, PxMeshGeometryFlags(), 1.0f, 1.0f, 1.0f);
//
//	// Create a PhysX actor
//	PxTransform transform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxIdentity));
//	PxRigidActor* actor = PxCreateStatic(*physics, transform, heightFieldGeom, *mMaterial);
//
//	mScene->addActor(*actor);
//
//	//// Create a controller manager
//	//PxControllerManager* manager = PxCreateControllerManager(*mScene);
//
//	//// Create a controller description
//	//PxCapsuleControllerDesc desc;
//	//desc.position = PxExtendedVec3(0.0f, 0.0f, 0.0f);
//	//desc.slopeLimit = 0.707f;
//	//desc.contactOffset = 0.1f;
//	//desc.stepOffset = 0.2f;
//	//desc.density = 10.0f;
//	//desc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
//
//	//// Create a controller
//	//PxController* controller = manager->createController(desc);
//
//	//// Move the controller
//	//PxVec3 displacement(1.0f, 0.0f, 0.0f);
//	//PxControllerFilters filters;
//	//controller->move(displacement, 0.01f, 1 / 60.0f, filters);
//}
//
//void createStack() {
//	float halfExtent = 10.0f;
//	PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
//	PxU32 size = 30;
//
//	//// Set the collision filtering for the dynamic object
//	//PxFilterData filterData;
//	//filterData.word0 = 1 << 0;  // Set the collision group to bit 0
//	//filterData.word1 = 1 << 1;  // Set the collision mask to bit 1
//	//shape->setQueryFilterData(filterData);
//	//shape->setSimulationFilterData(filterData);
//
//	for (PxU32 i = 0; i < size; i++)
//	{
//		for (PxU32 j = 0; j < size - i; j++)
//		{
//			PxTransform t(PxVec3(0));
//			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i) + 10, PxReal(i * 2 + 1), 20) * halfExtent);
//			PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
//			body->attachShape(*shape);
//			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
//			mScene->addActor(*body);
//		}
//	}
//	shape->release();
//}
//
////// Create a convex mesh approximation of the height field geometry
////PxConvexMeshDesc convexDesc;
////convexDesc.points.count = width * height;
////convexDesc.points.stride = sizeof(PxVec3);
////convexDesc.points.data = samples;
////PxConvexMesh* convexMesh = cooking->createConvexMesh(convexDesc, physics->getPhysicsInsertionCallback());
////
////// Create a convex mesh geometry with the convex mesh approximation of the height field
////PxConvexMeshGeometry convexGeom(convexMesh);
////
////// Create a shape with the convex mesh geometry and attach it to the actor
////PxShape* shape = actor->createShape(convexGeom, *mMaterial);
////shape->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f