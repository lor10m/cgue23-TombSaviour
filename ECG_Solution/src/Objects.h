#pragma once

#include "Utils/Utils.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Utils/Transform.h"
#include "Enemy.h"
#include "ShaderManager.h"
#include "Drawables/Cube.h"

class Objects {

private:
	PhysxScene* physxScene;
	Camera* camera;

	Terrain terrain;
	Character mummy;
	Enemy enemy;
	Model enemyModel;
	Model palmTree;
	Model pyramid;

	Shader terrainShader;
	Shader enemyShader;
	Shader palmTreeShader;
	Shader pyramidShader;


public:

	Objects(Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float dt);
	void createMummy();
	void createTerrain();
	void createEnemy();
	void createPyramid();
	void createPalmTree();
	void deleteObjects();
};