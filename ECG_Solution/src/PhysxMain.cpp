//#include <GLFW/glfw3.h>
//#include "PxPhysicsAPI.h"
//#include <iostream>
//#include "Utils.h"
//#include <vector>
//
//using namespace physx;
//void createStack(physx::PxMaterial* mMaterial, physx::PxPhysics* mPhysics, physx::PxScene* mScene);
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
//
//physx::PxPvd* mPvd = NULL;
//
//int main() {
//
//	// init physx
//	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
//	if (!mFoundation) throw("PxCreateFoundation failed!");
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
//
//	// create simulation
//	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//	physx::PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 50), *mMaterial);
//	mScene->addActor(*groundPlane);
//
//	createStack(mMaterial, mPhysics, mScene);
//
//	// run simulation
//	while (1) {
//		mScene->simulate(1.0f / 60.0f);
//		mScene->fetchResults(true);
//	}
//
//}
//
//void createStack(physx::PxMaterial* mMaterial, physx::PxPhysics* mPhysics, physx::PxScene* mScene) {
//	float halfExtent = 10.0f;
//	PxShape* shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
//	PxU32 size = 30;
//	PxTransform t(physx::PxVec3(0));
//
//	PxTransform localTm(physx::PxVec3(physx::PxReal(2) - physx::PxReal(1), physx::PxReal(3), 0) * halfExtent);
//	PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
//	body->attachShape(*shape);
//	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
//	mScene->addActor(*body);
//
//	/*PxShape* shape2;
//	body->getShapes(&shape2, 1, 1);*/
//	const PxGeometryHolder& geomHolder = shape->getGeometry();
//	const PxGeometry& geom = geomHolder.any();
//
//
//	//GET VERTICES AND INDICES
//	std::vector<float> vertices;
//	std::vector<float> indices;
//
//	if (geom.getType() == PxGeometryType::eTRIANGLEMESH) {
//		// Get the triangle mesh geometry
//		const PxTriangleMeshGeometry& meshGeom = static_cast<const PxTriangleMeshGeometry&>(geom);
//
//		// Get the triangle mesh data
//		const PxTriangleMesh* mesh = meshGeom.triangleMesh;
//		const PxVec3* meshVertices = mesh->getVertices();
//		const void* meshIndicesVoid = mesh->getTriangles();
//		const PxU32* meshIndices = reinterpret_cast<const PxU32*>(meshIndicesVoid);
//
//		// Get the number of vertices and indices in the mesh
//		const int numVertices = mesh->getNbVertices();
//		const int numIndices = mesh->getNbTriangles() * 3;
//		// Copy the vertices and indices into the output vectors
//		for (int j = 0; j < numVertices; ++j) {
//			vertices.push_back(meshVertices[j].x);
//			vertices.push_back(meshVertices[j].y);
//			vertices.push_back(meshVertices[j].z);
//		}
//
//		for (int j = 0; j < numIndices; ++j) {
//			indices.push_back(meshIndices[j]);
//		}
//	}
//
//	else if (geom.getType() == PxGeometryType::eBOX) {
//		// Get the box geometry
//		const PxBoxGeometry& boxGeom = geomHolder.box();
//
//		// Get the dimensions of the box
//		const PxVec3& halfExtents = boxGeom.halfExtents;
//
//		// Compute the vertices of the box
//		const float x = halfExtents.x;
//		const float y = halfExtents.y;
//		const float z = halfExtents.z;
//
//		const float verticesData[] = {
//			// Front
//			-x, -y, -z,
//			 x, -y, -z,
//			 x,  y, -z,
//			-x,  y, -z,
//
//			// Back
//			 x, -y,  z,
//			-x, -y,  z,
//			-x,  y,  z,
//			 x,  y,  z,
//
//			 // Top
//			 -x,  y, -z,
//			  x,  y, -z,
//			  x,  y,  z,
//			 -x,  y,  z,
//
//			 // Bottom
//			 -x, -y,  z,
//			  x, -y,  z,
//			  x, -y, -z,
//			 -x, -y, -z,
//
//			 // Left
//			 -x, -y,  z,
//			 -x, -y, -z,
//			 -x,  y, -z,
//			 -x,  y,  z,
//
//			 // Right
//			  x, -y, -z,
//			  x, -y,  z,
//			  x,  y,  z,
//			  x,  y, -z
//		};
//
//		const unsigned int indicesData[] = {
//			0, 1, 2,  0, 2, 3,  // Front
//			4, 5, 6,  4, 6, 7,  // Back
//			8, 9, 10, 8, 10, 11, // Top
//			12, 13, 14, 12, 14, 15, // Bottom
//			16, 17, 18, 16, 18, 19, // Left
//			20, 21, 22, 20, 22, 23 // Right
//		};
//
//		// Append the box vertices and indices to the output vectors
//		vertices.insert(vertices.end(), verticesData, verticesData + 24 * 3);
//		indices.insert(indices.end(), indicesData, indicesData + 36);
//	}
//
//	shape->release();
//}
//
////PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
////{
////    PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
////    dynamic->setAngularDamping(0.5f);
////    dynamic->setLinearVelocity(velocity);
////    gScene->addActor(*dynamic);
////    return dynamic;
////}
//
////#include <PxPhysicsAPI.h>
////
////	void getMeshDataFromActor(PxRigidDynamic * actor, std::vector<float>&vertices, std::vector<unsigned int>&indices) {
////		// Get the number of shapes attached to the actor
////		const int numShapes = actor->getNbShapes();
////
////		// Loop through each shape
////		for (int i = 0; i < numShapes; ++i) {
////			// Get the shape
////			PxShape* shape;
////			actor->getShapes(&shape, 1, i);
////
////			// Get the geometry of the shape
////			const PxGeometryHolder& geomHolder = shape->getGeometry();
////			const PxGeometry& geom = geomHolder.any();
////
////			// Check the type of geometry
////			switch (geom.getType()) {
////			case PxGeometryType::eTRIANGLEMESH:
////				// Get the triangle mesh geometry
////				const PxTriangleMeshGeometry& meshGeom = static_cast<const PxTriangleMeshGeometry&>(geom);
////
////				// Get the triangle mesh data
////				const PxTriangleMesh* mesh = meshGeom.triangleMesh;
////				const PxVec3* meshVertices = mesh->getVertices();
////				const PxU32* meshIndices = mesh->getTriangles();
////
////				// Get the number of vertices and indices in the mesh
////				const int numVertices = mesh->getNbVertices();
////				const int numIndices = mesh->getNbTriangles() * 3;
////
////				// Copy the vertices and indices into the output vectors
////				for (int j = 0; j < numVertices; ++j) {
////					vertices.push_back(meshVertices[j].x);
////					vertices.push_back(meshVertices[j].y);
////					vertices.push_back(meshVertices[j].z);
////				}
////
////				for (int j = 0; j < numIndices; ++j) {
////					indices.push_back(meshIndices[j]);
////				}
////				break;
////
////			case PxGeometryType::eBOX:
////				// Get the box geometry
////				const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
////
////				// Get the box dimensions
////				const PxVec3 boxDimensions = boxGeom.halfExtents;
////
////				// Define the box vertices
////				const PxVec3 boxVertices[] = {
////					PxVec3(boxDimensions.x, boxDimensions.y, boxDimensions.z),
////					PxVec3(-boxDimensions.x, boxDimensions.y, boxDimensions.z),
////					PxVec3(-boxDimensions.x, -boxDimensions.y, boxDimensions.z),
////					PxVec3(boxDimensions.x, -boxDimensions.y, boxDimensions.z),
////					PxVec3(boxDimensions.x, boxDimensions.y, -boxDimensions.z),
////					PxVec3(-boxDimensions.x, boxDimensions.y, -boxDimensions.z),
////					PxVec3(-boxDimensions.x, -boxDimensions.y, -boxDimensions.z),
////					PxVec3(boxDimensions.x, -boxDimensions.y, -boxDimensions.z)
////				};
////
////				// Define the box indices
////				const unsigned int boxIndices[] = {
////					0, 1, 2, 2, 3, 0,
////					4, 5, 1, 1, 0, 4,
////					7, 6, 5, 5, 4, 7,
////					3, 2, 6, 6, 7, 3,
////					4, 0, 3, 3, 7, 4,
////					1, 5, 6, 6,
