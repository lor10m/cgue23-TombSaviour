#include "Enemy.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>


Enemy::Enemy(const char* name, Model* model2, PhysxScene* physxScene, glm::vec3 scalingFactor, glm::vec3 position)
{
	// create PxController: 
	PxControllerManager* enegManager = PxCreateControllerManager(*physxScene->scene);
	PxCapsuleControllerDesc enecDesc;
	enecDesc.position = PxExtendedVec3{ position.x, position.y, position.z };
	enecDesc.contactOffset = 0.05f;
	enecDesc.height = 10.0f;
	enecDesc.radius = 1.0f;
	enecDesc.stepOffset = 0.2f;
	enecDesc.slopeLimit = 0.2f;
	enecDesc.upDirection = PxVec3(0.0f, 1.0f, 0.0f);
	enecDesc.material = physxScene->material;
	pxChar = enegManager->createController(enecDesc);
	pxChar->getActor()->setName(name); 
	model = model2;
	scale = scalingFactor;
	setPosition();
}

void Enemy::move(glm::vec3 dir, float speed, float dt)
{
	PxVec3 targetPosition = PxVec3(dir.x, -100.0f, dir.z);
	PxVec3 currPos = PxVec3(charPosition.x, charPosition.y, charPosition.z);
	
	// calculate rotation based on moving direction
	PxVec3 movementDirection = targetPosition - currPos;
	movementDirection.y = 0;
	float angle = atan2(movementDirection.z, movementDirection.x) * 180.0f / PxPi;
	float x = glm::radians(-angle + 60.0f);

	Transform rotationMatrix;
	rotationMatrix.rotate(glm::vec3(0.0, x, 0.0));
	glmRotationMatrix = rotationMatrix.getMatrix();

	// calculate position based on target position
	PxVec3 displacement = targetPosition - currPos;
	displacement.normalize();
	displacement *= (speed * dt); //TODO 

	pxChar->move(displacement, 0.001f, dt, physx::PxControllerFilters());

	setPosition();
}

void Enemy::setPosition() {
	physx::PxExtendedVec3 pos = pxChar->getFootPosition(); //getFootPosition()
	charPosition.x = pos.x;
	charPosition.y = pos.y;
	charPosition.z = pos.z;

	model->physxTransform = glm::vec3(charPosition.x/ scale.x, charPosition.y/ scale.y, charPosition.z/ scale.z);
	model->physxRotate = glmRotationMatrix;

	//printPosition();
}

glm::vec3 Enemy::getPosition() {
	return charPosition;
}

void Enemy::printPosition() { 
	std::cout << "\nCharPos: " << charPosition.x << " " << charPosition.y << " " << charPosition.z << std::endl;
}