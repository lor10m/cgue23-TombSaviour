#include "PhysxScene.h"
#include <postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Utils/Transform.h"
#include "Utils/PhysxCallbacks.h"

PhysxScene::PhysxScene(GLFWwindow* window)
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
	PxInitExtensions(*physics, pvd);

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.simulationEventCallback = this;
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
	PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 50), *material);
	scene->addActor(*groundPlane);

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	if (!cooking) {
		std::cerr << ("Failed to init cooking") << std::endl;
	}

	//glfwSetWindowUserPointer(window, this);
	//glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
 //      static_cast<PhysxScene*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(button, action, mods, camera);});
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

	PxHeightField* heightField = cooking->createHeightField(heightFieldDesc, physics->getPhysicsInsertionCallback());
	PxHeightFieldGeometry heightFieldGeom(heightField, PxMeshGeometryFlags(), 1.0f, 1.0f, 1.0f);
	PxTransform transform(PxVec3(-width / 2, 0.0f, -width / 2), PxQuat(PxIdentity));
	PxRigidActor* actor = PxCreateStatic(*physics, transform, heightFieldGeom, *material);

	scene->addActor(*actor);
}

void PhysxScene::createModel(const char* name, std::vector<unsigned int> indices, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotate)
{
	PxTriangleMeshDesc meshDesc;

	meshDesc.points.count = vertices.size();
	meshDesc.points.stride = sizeof(glm::vec3);
	meshDesc.points.data = vertices.data();
	meshDesc.triangles.count = indices.size() / 3;
	meshDesc.triangles.stride = sizeof(unsigned int) * 3;
	meshDesc.triangles.data = indices.data();

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
	if (!status)
	{
		std::cout << "Cooking error" << std::endl;
		return;
	}
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* triangleMesh = physics->createTriangleMesh(readBuffer);

	PxMeshScale pxMeshScale;
	PxQuat quatX(rotate.x, PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat quatY(rotate.y, PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat quatZ(rotate.z, PxVec3(0.0f, 0.0f, 1.0f));
	PxQuat combinedQuat = quatZ * quatY * quatX;		// rotation order: x,y,z
	pxMeshScale.rotation = PxQuat(combinedQuat);
	pxMeshScale.scale = PxVec3(scale.x, scale.y, scale.z);

	PxTriangleMeshGeometry geometry(triangleMesh, pxMeshScale);

	PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.1f);
	PxShape* shape = physics->createShape(geometry, *material);
	
	PxTransform transform(PxVec3(translate.x, translate.y, translate.z), PxIdentity);
	shape->setLocalPose(transform);

	PxRigidStatic* staticActor = physics->createRigidStatic(PxTransform(PxIdentity));

	staticActor->attachShape(*shape);
	staticActor->setGlobalPose(PxTransform(PxIdentity));

	staticActor->setName(name);

	scene->addActor(*staticActor);

};

void PhysxScene::setCharacter(Character* character)
{
	mummy = character;
}

void PhysxScene::createCactus(unsigned int index, glm::vec3 size, glm::vec3 position)
{
	PxBoxGeometry boxGeometry(PxVec3(size.x, size.y, size.z));
	PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.1f);
	PxShape* shape = physics->createShape(boxGeometry, *material);

	PxRigidStatic* staticActor = physics->createRigidStatic(PxTransform(PxIdentity));
	staticActor->attachShape(*shape);
	PxTransform transform(PxVec3(position.x, position.y, position.z), PxIdentity);
	staticActor->setGlobalPose(transform);
	staticActor->setName("cactus");
	scene->addActor(*staticActor);

	StaticActor cactus;
	cactus.index = index;
	cactus.actor = staticActor;
	cacti[index] = cactus;
}

void PhysxScene::createSpike(unsigned int index, glm::vec3 size, glm::vec3 position)
{
	//TODO calculate rotation

	PxBoxGeometry boxGeometry(PxVec3(size.x, size.y, size.z));
	PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.1f);
	PxShape* shape = physics->createShape(boxGeometry, *material);

	PxRigidDynamic* dynamicActor = physics->createRigidDynamic(PxTransform(PxIdentity));

	dynamicActor->attachShape(*shape);
	PxTransform transform(PxVec3(position.x, position.y, position.z), PxIdentity);
	dynamicActor->setGlobalPose(transform);
	dynamicActor->setName("spike");

	DynamicActor spike;
	spike.index = index;
	spike.actor = dynamicActor;
	spikes.push_back(spike);
}

void PhysxScene::simulate(GLFWwindow* window, Camera* camera, float timeStep, std::map<unsigned int, SpikeStruct>& spikeStruct, std::map<unsigned int, CactusStruct>& cactusStruct)
{
	scene->simulate(timeStep);
	scene->fetchResults(true);

	mouseButtonCallback(window, camera);

	for (DynamicActor spike : spikes) {
		if (spike.isThrownOrPickedUp) {
			PxVec3 newPos = spike.actor->getGlobalPose().p;
			//TODO: set rotation: spike.actor->getGlobalPose().q
			spikeStruct[spike.index].scale = glm::vec3(0.1f, 0.1f, 0.1f);
			spikeStruct[spike.index].translate = glm::vec3(newPos.x, newPos.y, newPos.z);
			spikeStruct[spike.index].render = true;
		}
	}
	for (auto& pair : cacti) {
		//cactusStruct[pair.second.index].modelMatrix = pair.second.actor->getGlobalPose().p
		if (pair.second.isThrownOrPickedUp) {
			cactusStruct.erase(pair.second.index);
			cacti.erase(pair.second.index);
			break;
		}
	}
}


//TODO make mouseButtonCallback instead
void PhysxScene::mouseButtonCallback(GLFWwindow* window, Camera* camera)
{
	// PICK UP CACTUS
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		if (mouse_left_released) {
			std::cout << "pick up object" << std::endl;
			pickUpNearestObject(camera);
			mouse_left_pressed = true;
			mouse_left_released = false;
		}

	}
	// THROW SPIKE
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		if (mouse_right_released) {
			std::cout << "throw object" << std::endl;
			mouse_right_pressed = true;
			mouse_right_released = false;
			if (pickedUpSpikes > 0) {
				throwSpike(camera);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE) {
		mouse_right_released = true;
		mouse_right_pressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE) {
		mouse_left_released = true;
		mouse_left_pressed = false;
	}
}


void PhysxScene::pickUpNearestObject(Camera* camera)
{
	float pickUpDistance = 2.0f;
	glm::vec3 mummyPos = mummy->getPosition();

	for (auto& pair : cacti)
	{
		PxRigidStatic* object = pair.second.actor;
		PxTransform objectPosition = object->getGlobalPose();
		float distanceToMummy = sqrt(pow(objectPosition.p.x - mummyPos.x, 2) + pow(objectPosition.p.y - mummyPos.y, 2));

		if (distanceToMummy <= pickUpDistance && !pickedUp)
		{
			scene->removeActor(*object);
			pair.second.isThrownOrPickedUp = true;
			pickedUpSpikes += spickesPerCactus;
			break;
		}
	}
}

void PhysxScene::throwSpike(Camera* camera)
{
	pickedUpSpikes--;

	// Get one of the spikes
	spikes[thrownSpikes].isThrownOrPickedUp = true;
	PxRigidDynamic* object = spikes[thrownSpikes].actor;
	thrownSpikes++;

	glm::vec3 origin = camera->getCameraPosition();
	glm::vec3 cameraForward = camera->getDirection();
	float distanceFromCamera = 2.0f;
	glm::vec3 desiredPos = origin + cameraForward * distanceFromCamera;
	PxVec3 objectPos = PxVec3(desiredPos.x, desiredPos.y, desiredPos.z);

	// Set position of spike to position of Characters "hand", a little in front of camera
	PxTransform objPos = object->getGlobalPose();
	objPos.p = objectPos;
	object->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	object->setGlobalPose(objPos);
	scene->addActor(*object); // add the spike to the scene

	PxVec3 rayOrigin = PxVec3(origin.x, origin.y, origin.z);
	PxVec3 throwDirection = -(rayOrigin - objectPos).getNormalized(); //TODO

	// throw spike
	object->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	object->addForce(throwDirection * 100, PxForceMode::eIMPULSE);
}

PxScene* PhysxScene::getScene()
{
	return scene;
}

void PhysxScene::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	std::cout << "collide" << std::endl;

	for (PxU32 i = 0; i < nbPairs; i++) {
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "collide" << std::endl;
		}
	}
}

void PhysxScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	std::cout << "trigger" << std::endl;
}

void PhysxScene::deleteScene()
{
	//TODO release, delete all
	//scene->release();
	//physics->release();
	//foundation->release();
}