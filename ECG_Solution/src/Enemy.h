#pragma once

#include <PxPhysicsAPI.h>
#include "Drawables/Model.h"
#include "Utils/Transform.h"
#include "PhysxScene.h"

using namespace physx;


class Enemy : public PxUserControllerHitReport
{

private:

	Assimp::Importer punchingImporter;
	const aiScene* punchingScene = NULL;

	Assimp::Importer killedImporter;
	const aiScene* killedScene = NULL;

	unsigned int hitCounter = 0;

	bool colliding = false;

	glm::vec3 scale;
	glm::mat4 glmRotationMatrix = glm::mat4(1.0f);

	double currTime;
	double startTime = 0.0;

public:

	PxController* pxChar = nullptr;
	glm::vec3 charPosition;

	std::shared_ptr<Model> model;

	bool isDead = false;
	bool shouldBeDeleted = false;

	Enemy() {};
	Enemy(unsigned int id, std::shared_ptr<Model> model, PhysxScene* physxScene, PxControllerManager* enegManager, glm::vec3 scale, glm::vec3 position);
	glm::vec3 getPosition();

	void move(Shader& enemyShader , glm::mat4 modelMatrix, float currentTime, glm::vec3 dir, float speed, float dt);
	void killEnemy();
	void setPosition();
	void printPosition();

	virtual void onShapeHit(const PxControllerShapeHit& shapeHit) {}
	virtual void onControllerHit(const PxControllersHit&);
	virtual void onObstacleHit(const PxControllerObstacleHit&) { }
};