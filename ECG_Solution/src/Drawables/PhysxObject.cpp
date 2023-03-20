#include "PhysxObject.h"

using namespace physx;

PxDefaultAllocator mDefaultAllocatorCallback;
PxDefaultErrorCallback mDefaultErrorCallback;
PxDefaultCpuDispatcher* mDispatcher = NULL;
PxTolerancesScale mToleranceScale;
PxFoundation* mFoundation = NULL;
PxPhysics* mPhysics = NULL;
PxMaterial* mMaterial = NULL;
PxPvd* mPvd = NULL;
PxScene* mScene = NULL;

PhysxObject::PhysxObject(glm::vec3 location) {
	initPhysics(location);
}

void PhysxObject::initPhysics(glm::vec3 location) {
	// init physx
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
	if (!mFoundation) throw("PxCreateFoundation failed!");
	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	mToleranceScale.length = 100;        // typical length of an object
	mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);

	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = mPhysics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}


	// create simulation
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 50), *mMaterial);
	mScene->addActor(*groundPlane);

	createObject(mMaterial, mPhysics, location);

	// run simulation
	//while (1) {
	//	mScene->simulate(1.0f / 60.0f);
	//	mScene->fetchResults(true);
	//}
}

void PhysxObject::createObject(physx::PxMaterial* mMaterial, physx::PxPhysics* mPhysics, glm::vec3 location) {
	float halfExtent = 1.0f;
	PxShape* shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	PxU32 size = 30;
	PxTransform t(physx::PxVec3(0));

	PxTransform localTm(physx::PxVec3(physx::PxReal(location.x) - physx::PxReal(location.y), physx::PxReal(location.z), 0) * halfExtent);
	PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	mScene->addActor(*body);

	/*PxShape* shape2;
	body->getShapes(&shape2, 1, 1);*/
	const PxGeometryHolder& geomHolder = shape->getGeometry();
	const PxGeometry& geom = geomHolder.any();

	//GET VERTICES AND INDICES

	if (geom.getType() == PxGeometryType::eTRIANGLEMESH) {
		// Get the triangle mesh geometry
		const PxTriangleMeshGeometry& meshGeom = static_cast<const PxTriangleMeshGeometry&>(geom);

		// Get the triangle mesh data
		const PxTriangleMesh* mesh = meshGeom.triangleMesh;
		const PxVec3* meshVertices = mesh->getVertices();
		const void* meshIndicesVoid = mesh->getTriangles();
		const PxU32* meshIndices = reinterpret_cast<const PxU32*>(meshIndicesVoid);

		// Get the number of vertices and indices in the mesh
		const int numVertices = mesh->getNbVertices();
		const int numIndices = mesh->getNbTriangles() * 3;
		// Copy the vertices and indices into the output vectors
		for (int j = 0; j < numVertices; ++j) {
			vertices.push_back(glm::vec3(meshVertices[j].x, meshVertices[j].y, meshVertices[j].z));
		}

		for (int j = 0; j < numIndices; ++j) {
			indices.push_back(meshIndices[j]);
		}
	}

	else if (geom.getType() == PxGeometryType::eBOX) {
		// Get the box geometry
		const PxBoxGeometry& boxGeom = geomHolder.box();

		// Get the dimensions of the box
		const PxVec3& halfExtents = boxGeom.halfExtents;

		// Compute the vertices of the box
		const float boxWidth = halfExtents.x;
		const float boxHeight = halfExtents.y;
		const float boxDepth = halfExtents.z;

		vertices = {
			// front
			glm::vec3(-boxWidth, -boxHeight, boxDepth),
			glm::vec3(boxWidth, -boxHeight, boxDepth),
			glm::vec3(boxWidth, boxHeight, boxDepth),
			glm::vec3(-boxWidth, boxHeight, boxDepth),
			// back
			glm::vec3(boxWidth, -boxHeight, -boxDepth),
			glm::vec3(-boxWidth, -boxHeight, -boxDepth),
			glm::vec3(-boxWidth, boxHeight, -boxDepth),
			glm::vec3(boxWidth, boxHeight, -boxDepth)
		};

		indices = {
			//front
			0, 1, 2,
			2, 3, 0,
			//back
			4, 5, 6,
			6, 7, 4,
			//right
			1, 4, 7,
			7, 2, 1,
			//left
			5, 0, 3,
			3, 6, 5,
			//top
			6, 3, 2,
			2, 7, 6,
			//bottom
			5, 4, 1,
			1, 0, 5
		};
		std::cout << boxWidth << std::endl;
	}
	
	normals = {
		//frontside
		glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			//backside
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			//right
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			//left
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),

			//top
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			//bottom
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f)
	};

	//create Vertex Array Object and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create Vertex Buffer Object and bind it and store vertices on it
	glGenBuffers(1, &VBO); //1 object and pointing to reference
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //make object current object
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	//create Normals Vertex Buffer Object and bind it to position 1
	glGenBuffers(1, &NormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	//create Edge Buffer Object and bind it and store indices on it
	glGenBuffers(1, &EBO); //1 object and pointing to reference
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //make object current object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //configure it so that opengl knows how to read it
	glEnableVertexAttribArray(0); //0 = position of VA

	glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //configure it so that opengl knows how to read it
	glEnableVertexAttribArray(1); //1 = position of VA

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	shape->release();
}

void PhysxObject::draw() {
	//mScene->simulate(1.0f / 60.0f);
	//mScene->fetchResults(true);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::vec3 PhysxObject::getPosition() {

	PxVec3 position;
	PxQuat orientation;

	const PxU32 bufferSize = 128;
	PxActor* buffer[bufferSize];
	PxU32 startIndex = 0;
	PxU32 nbActors = mScene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC, buffer, bufferSize, startIndex); // get all actors i

	//PxActor** actors = new PxActor * [nbActors];

	// iterate over all actors and do something with them
	for (PxU32 i = 0; i < nbActors; i++) {
		PxActor* actor = buffer[i];
		if (actor->is<PxRigidDynamic>()) {
			PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(actor);
			// get the global pose of the rigid dynamic
			PxTransform transform = rigidDynamic->getGlobalPose();
			position = transform.p;
			orientation = transform.q;
		}
		else if (actor->is<PxRigidStatic>()) {
			PxRigidStatic* rigidStatic = static_cast<PxRigidStatic*>(actor);
			// get the global pose of the rigid static
			PxTransform transform = rigidStatic->getGlobalPose();
			position = transform.p;
			orientation = transform.q;
		}
	}
	//std::cout << position.x << std::endl;
	//std::cout << position.y << std::endl;
	//std::cout << position.z << std::endl;

	return glm::vec3(position.x, position.y, position.z);
}