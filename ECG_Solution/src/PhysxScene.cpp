#include "PhysxScene.h"

PhysxScene::PhysxScene()
{
	// init physx
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
	if (!foundation) throw("PxCreateFoundation failed!");

	//Physx Visual Debugger
	pvd = PxCreatePvd(*foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//create Physx
	toleranceScale.length = 100;        // typical length of an object
	toleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale, true, pvd);

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// create simulation
	material = physics->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 50), *material);
	scene->addActor(*groundPlane);

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	if (!cooking) {
		std::cerr << ("Failed to init cooking") << std::endl;
	}
}

void PhysxScene::createTerrain(const char* heightmapPath)
{
	int width, height, channels;
	// Load the heightmap image
	unsigned char* image = stbi_load(heightmapPath, &width, &height, &channels, STBI_grey);

	// Create the height field samples buffer
	PxHeightFieldSample* samples = new PxHeightFieldSample[width * height];

	// Set the height field sample data from the image data
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			samples[y * width + x].height = static_cast<PxI16>(image[y * width + x]);
		}
	}

	// Create the height field descriptor
	PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbColumns = width;
	heightFieldDesc.nbRows = height;
	heightFieldDesc.samples.data = samples;
	heightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);

	// Create the height field
	PxHeightField* heightField = cooking->createHeightField(heightFieldDesc, physics->getPhysicsInsertionCallback());

	// Create the height field geometry
	PxHeightFieldGeometry heightFieldGeom(heightField, PxMeshGeometryFlags(), 1.0f, 1.0f, 1.0f);

	// Create a PhysX actor
	PxTransform transform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxIdentity));
	PxRigidActor* actor = PxCreateStatic(*physics, transform, heightFieldGeom, *material);

	scene->addActor(*actor);
}

void PhysxScene::createPlayer(PhysxCamera playerCam)
{
	// create player actor
	PxTransform playerTransform(PxVec3(0.0f, 50.0f, 0.0f));
	PxBoxGeometry playerGeometry(PxVec3(1.0f, 2.0f, 1.0f));
	playerActor = PxCreateDynamic(*physics, playerTransform, playerGeometry, *material, 1.0f);
	playerActor->setAngularDamping(0.5f);
	playerActor->setMass(10.0f);
	scene->addActor(*playerActor);

	playerCamera = playerCam;
	
	//create cam offset
	//attach camera to player
	PxVec3 cameraOffset(0.0f, 1.5f, 0.0f);
	PxTransform transform = playerTransform.transform(PxTransform(cameraOffset));

	//transform camera
	playerCamera.setTransform(playerTransform.transform(PxTransform(cameraOffset)));

	//// create camera
	//camera = new Camera();
	//camera->setAspect(1.33f);
	//camera->setFOV(45.0f);
	//camera->setNearClip(0.1f);
	//camera->setFarClip(1000.0f);
}

void PhysxScene::simulate(GLFWwindow* window, float timeStep)
{
	scene->simulate(timeStep);
	scene->fetchResults(true);

	//// handle input to move player
	//PxVec3 playerVelocity(0.0f);

	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	//	playerVelocity += playerCamera.getForwardVector() * 10.0f;
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	//	playerVelocity -= playerCamera.getForwardVector() * 10.0f;
	//}
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	//	playerVelocity -= playerCamera.getRightVector() * 10.0f;
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	//	playerVelocity += playerCamera.getRightVector() * 10.0f;
	//}

	//playerVelocity.y = playerActor->getLinearVelocity().y;
	//playerActor->setLinearVelocity(playerVelocity);

	//// update camera position and orientation
	//PxTransform playerTransform = playerActor->getGlobalPose();
	//PxVec3 cameraOffset(0.0f, 1.5f, 0.0f);

	//playerCamera.setTransform(playerTransform.transform(PxTransform(cameraOffset)));
	//playerCamera.lookAt(playerTransform.p + playerCamera.getForwardVector() * 10.0f, playerTransform.p + PxVec3(0.0f, 1.5f, 0.0f));

}