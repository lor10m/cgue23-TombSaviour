#pragma once

#include "Utils.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"

using namespace physx;


class PhysxCamera {
public:

    PhysxCamera();

    PhysxCamera(double fov, double aspect_ratio, double near, double far);

    void setAspect(float aspectRatio);

    void setFOV(float fov);

    void setNearClip(float nearClip);

    void setFarClip(float farClip);

    void setTransform(PxTransform& transform);

    void lookAt(PxVec3& target, PxVec3& up);

    PxTransform& getTransform();

    PxVec3 getForwardVector();

    PxVec3 getRightVector();

    glm::mat4 getViewMatrix();

private:
    PxTransform m_transform;
    float m_aspectRatio;
    float m_fov;
    float m_nearClip;
    float m_farClip;
};