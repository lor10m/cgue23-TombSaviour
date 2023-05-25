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
#include "Utils/GlobalVariables.h"

class Objects {

private:
	PhysxScene* physxScene;
	Camera* camera;
	
	
	Hdu hduObject;

	Terrain terrain;
	Enemy enemy;
	Model enemyModel;
	Model palmTree;
	Model pyramid;

	Model pointLightCube;

	Shader terrainShader;
	Shader enemyShader;
	Shader palmTreeShader;
	Shader pyramidShader;
	Shader cactusShader;
	Shader spikeShader;

	Shader lightCubeShader;

	Character mummy;
	Texture cactusTexture;
	Shader hduShader;

	unsigned int spikeCounter = 0;
	unsigned int cactiCounter = 0;

	std::map<unsigned int, SpikeStruct> spikes;
	std::map<unsigned int, CactusStruct> cacti;
	

	unsigned int numCacti = 3;
	unsigned int numSpikes = 3 * numCacti;
	void createSpike();

	double physxDeltaTime = 0.0;
	
public:

	Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float currentTime, float dt);
	void createMummy(GLFWwindow* window);
	void createTerrain();
	void createEnemy();
	void createPyramid();
	void createPalmTree();
	void createPointLightCube();
	void createHduObject(GLFWwindow* window);
	void deleteObjects();
	void createCactus(glm::vec3 position);
};