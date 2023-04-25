#pragma once
using namespace physx;

// define the collision callback function
class MyContactCallback : public PxContactModifyCallback
{
	virtual void onContactModify(PxContactModifyPair* const pairs, PxU32 count)
	{
		std::cout << "collision" << std::endl;
	}
};


class MyCollisionCallback : public PxSimulationEventCallback {
    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {
        std::cout << "collision" << std::endl;
        for (PxU32 i = 0; i < nbPairs; i++) {
            const PxContactPair& cp = pairs[i];
            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                std::cout << "collision" << std::endl;
            }
        }
    }

    void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
    void onWake(PxActor**, PxU32) override {}
    void onSleep(PxActor**, PxU32) override {}
    void onTrigger(PxTriggerPair*, PxU32) override {}
    void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
};