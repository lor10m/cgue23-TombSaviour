#pragma once

#include "Utils/Utils.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Utils/Transform.h"
#include "Enemy.h"
#include "ShaderManager.h"
#include "Drawables/Cube.h"
#include "Hdu.h"

class Objects {

private:
	PhysxScene* physxScene;
	Camera* camera;
	
	
	Hdu hduObject;

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
	Shader hduShader;


public:

	Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float dt);
	void createMummy();
	void createTerrain();
	void createEnemy();
	void createPyramid();
	void createPalmTree();
	void createHduObject(GLFWwindow* window);
	void deleteObjects();

};