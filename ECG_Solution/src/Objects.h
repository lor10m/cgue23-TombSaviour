#pragma once

#include "Utils/Utils.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "Character.h"
#include "Utils/Transform.h"
#include "Enemy.h"
#include "ShaderManager.h"
#include "Drawables/Cube.h"
#include "Utils/GlobalVariables.h"

class Objects {

private:
	PhysxScene* physxScene;
	Camera* camera;

	Terrain terrain;
	Enemy enemy;
	Model enemyModel;
	Model palmTree;
	Model pyramid;

	Shader terrainShader;
	Shader enemyShader;
	Shader palmTreeShader;
	Shader pyramidShader;
	Shader cactusShader;
	Shader spikeShader;

	Character mummy;
	Texture cactusTexture;

	unsigned int spikeCounter = 0;
	unsigned int cactiCounter = 0;

	std::map<unsigned int, SpikeStruct> spikes;
	std::map<unsigned int, CactusStruct> cacti;
	

	unsigned int numCacti = 3;
	unsigned int numSpikes = 5 * numCacti;
	void createSpike();


public:

	Objects(Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float dt);
	void createMummy();
	void createTerrain();
	void createEnemy();
	void createPyramid();
	void createPalmTree();
	void deleteObjects();
	void createCactus(glm::vec3 position);
};