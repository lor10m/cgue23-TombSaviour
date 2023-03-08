#include <GLFW/glfw3.h>
#include "PxPhysicsAPI.h"

using namespace physx;
void createStack(physx::PxMaterial* mMaterial, physx::PxPhysics* mPhysics, physx::PxScene* mScene);

// declare variables
physx::PxDefaultAllocator      mDefaultAllocatorCallback;
physx::PxDefaultErrorCallback  mDefaultErrorCallback;
physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
physx::PxTolerancesScale       mToleranceScale;

physx::PxFoundation* mFoundation = NULL;
physx::PxPhysics* mPhysics = NULL;

physx::PxScene* mScene = NULL;
physx::PxMaterial* mMaterial = NULL;

physx::PxPvd* mPvd = NULL;

int main() {

    // init physx
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
    if (!mFoundation) throw("PxCreateFoundation failed!");
    mPvd = PxCreatePvd(*mFoundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    mToleranceScale.length = 100;        // typical length of an object
    mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);

    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
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

    createStack(mMaterial, mPhysics, mScene);

    // run simulation
    while (1) {
        mScene->simulate(1.0f / 60.0f);
        mScene->fetchResults(true);
    }

}

void createStack(physx::PxMaterial *mMaterial, physx::PxPhysics *mPhysics, physx::PxScene *mScene) {
    float halfExtent = .5f;
    physx::PxShape* shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
    physx::PxU32 size = 30;
    physx::PxTransform t(physx::PxVec3(0));
    for (physx::PxU32 i = 0; i < size; i++) {
        for (physx::PxU32 j = 0; j < size - i; j++) {
            physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
            physx::PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
            body->attachShape(*shape);
            physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
            mScene->addActor(*body);
        }
    }
    shape->release();
}

//PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
//{
//    PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
//    dynamic->setAngularDamping(0.5f);
//    dynamic->setLinearVelocity(velocity);
//    gScene->addActor(*dynamic);
//    return dynamic;
//}