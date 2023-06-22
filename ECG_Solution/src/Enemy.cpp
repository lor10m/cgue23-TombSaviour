#include "Enemy.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>


Enemy::Enemy(unsigned int id, std::shared_ptr<Model> model2, PhysxScene* physxScene, PxControllerManager* enegManager, glm::vec3 scalingFactor, glm::vec3 position)
{
	scene = physxScene;
	PxCapsuleControllerDesc enecDesc;
	enecDesc.position = PxExtendedVec3{ position.x, position.y, position.z };
	enecDesc.contactOffset = 0.05f;
	enecDesc.height = 10.0f;
	enecDesc.radius = 1.0f;
	enecDesc.stepOffset = 0.2f;
	enecDesc.slopeLimit = 0.2f;
	enecDesc.upDirection = PxVec3(0.0f, 1.0f, 0.0f);
	enecDesc.material = physxScene->material;
	enecDesc.reportCallback = this;
	pxChar = enegManager->createController(enecDesc);
	pxChar->getActor()->setName("enemy");
	pxChar->getActor()->userData = (void*)id;
	model = model2;
	scale = scalingFactor;

	punchingScene = punchingImporter.ReadFile("assets/models/Punching.dae", NULL);
	killedScene = killedImporter.ReadFile("assets/models/Dying.dae", NULL);

	setPosition();
}

void Enemy::move(Shader& enemyShader,glm::mat4 modelMatrix, float currentTime, glm::vec3 dir, float speed, float dt)
{
	if (shouldBeDeleted) { return; }
	if (isDead) {
		currTime = glfwGetTime();
		currentTime = glfwGetTime() - 0.2;
		if (startTime == 0.0) {
			model->animation = killedScene->mAnimations[0];
			model->isDeadEnemy = true;
			startTime = glfwGetTime();
		}
		else if (currTime - startTime >= 1.2) {
			killEnemy();
		}

	}

	enemyShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelMatrix);
	enemyShader.setUniform1i("withShadow", false);
	enemyShader.setUniform1i("videoWall", false);

	std::vector<glm::mat4> transformationMatrices;
	model->getBoneTransforms(currentTime, glm::mat4(1.0f), transformationMatrices);
	for (unsigned int i = 0; i < transformationMatrices.size(); i++) {
		glm::mat4 mat = transformationMatrices[i];
		enemyShader.setUniformMatrix4fv("bones[" + std::to_string(i) + "]", 1, GL_FALSE, mat);
	}
	model->draw(&enemyShader);


	if (!isDead) {
		colliding = false;

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

		if (!colliding) {
			model->animation = nullptr; // use default animation if enemy is not colliding with mummy
		}
	}
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
	
	//<< "\nCharPos: " << charPosition.x << " " << charPosition.y << " " << charPosition.z << std::endl;
}

void Enemy::killEnemy() 
{
	std::cout << "enemy dead" << std::endl;
	shouldBeDeleted = true;
}

void Enemy::onControllerHit(const physx::PxControllersHit&)
{
	hitCounter++;
	colliding = true;
	model->animation = punchingScene->mAnimations[0];

	if (hitCounter == 30) {		// change hitCounter
		// Leben abziehen
		scene->decreaseMummyLive();
		hitCounter = 0;
		std::cout << "controller hit " << std::endl;
	}
}