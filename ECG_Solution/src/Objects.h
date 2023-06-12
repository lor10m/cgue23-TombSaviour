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

struct EnemyStruct {
	unsigned int id;
	std::shared_ptr<Model> enemyModel = nullptr;
	std::shared_ptr<Enemy> enemy = nullptr;
	glm::mat4 modelMatrix;
};

class Objects {

private:
	PhysxScene* physxScene;
	PxControllerManager* controllerManager;
	Camera* camera;

	Hdu hduObject;

	Terrain terrain;
	Model palmTree;
	Model pyramid;
	Model pointLightCube;

	unsigned int vao;
	unsigned int vbo;
	unsigned int tangentsVBO;
	unsigned int bitangentsVBO;

	Shader testCubeShader;

	Shader terrainShader;
	Shader palmTreeShader;
	Shader pyramidShader;
	Shader cactusShader;
	Shader spikeShader;
	Shader lightCubeShader;

	Character mummy;
	Texture cactusTexture;
	Shader hduShader;

	Shader enemyShader;
	//std::shared_ptr<Model> enemyModel = nullptr;
	//std::shared_ptr<Enemy> enemy = nullptr;
	//Enemy enemy;

	unsigned int spikeCounter = 0;
	unsigned int cactiCounter = 0;
	unsigned int enemyCounter = 1;

	std::vector<unsigned int> deadEnemyIndices;


	std::map<unsigned int, EnemyStruct> enemies;
	std::map<unsigned int, SpikeStruct> spikes;
	std::map<unsigned int, CactusStruct> cacti;
	
	unsigned int numEnemies = 2;
	unsigned int numCacti = 3;
	unsigned int numSpikes = 10 * numCacti;
	void createSpike();

	double physxDeltaTime = 0.0;
	
public:

	Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene);
	void render(GLFWwindow* window, float currentTime, float dt, bool normalMapping);
	void createMummy(GLFWwindow* window);
	void createTerrain();
	void createEnemy(glm::vec3 position);
	void createPyramid();
	void createPalmTree();
	void createPointLightCube();
	void createTestCube();
	void createHduObject(GLFWwindow* window);
	void deleteObjects();
	void createCactus(glm::vec3 position);
};