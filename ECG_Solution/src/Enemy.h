#pragma once

#include <PxPhysicsAPI.h>
#include "Drawables/Model.h"
#include "Utils/Transform.h"
#include "PhysxScene.h"

using namespace physx;


class Enemy {

private:
	glm::vec3 scale;
	glm::mat4 glmRotationMatrix = glm::mat4(1.0f);

public:
	PxController* pxChar = nullptr;
	glm::vec3 charPosition;

	Model* model;

	Enemy() {};
	Enemy(Model* model, PhysxScene* physxScene, glm::vec3 scale, glm::vec3 position);
	glm::vec3 getPosition();

	void move(glm::vec3 dir, float speed, float dt);
	void setPosition();
	void printPosition();

};